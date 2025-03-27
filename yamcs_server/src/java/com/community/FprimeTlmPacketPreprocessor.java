package com.community;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.concurrent.atomic.AtomicInteger;

import org.yamcs.TmPacket;
import org.yamcs.YConfiguration;
import org.yamcs.tctm.AbstractPacketPreprocessor;
import org.yamcs.utils.TimeEncoding;

/* Expected input:
- 0xDEADBEEF 
- 4 bytes packet size (without checksum but with packet type?) 
- packet
    - 4 bytes packet type = 0x0001 for TM packets 
    - 4 bytes TMpacket ID 
    - 11 bytes time tag to ignore, we use the wall clock time instead 
    - actual packet payload to return 
- 4 bytes checksum */

/**
 * FprimePreprocessor is the class which extracts the telemetry from Fprime TM packets by stripping it clean from 
 * start word, length field, time tag, and checksum. It retains the packet type because the event service has events go through the same 
 * depacketization process, and the packet ID because it translates to actual TlmPacketizer packet ID or eventID depending on packet type.  
 * 
 * TODO:
 * - improve error handling, e.g. with YAMCS instance events
 * - Add CRC-32 check (must comply with https://www.lammertbies.nl/comm/info/crc-calculation)
 * - Add processing of time tag
 * 
 * @author GM
 **/
public class FprimeTlmPacketPreprocessor extends AbstractPacketPreprocessor {
    //Start word of the Fprime packet protocol
    private static final int SYNC_WORD = 0xDEADBEEF;
    //Number of bytes for the sync word. 
    private static final int SYNC_LENGTH = 4; //Warning: Only change in concert with the getInt(), getShort() here!
    //Number of bytes for the packet type field. Comes from fprime's FwPacketDescriptorType in config/FpConfig.h. 
    private static final int PACKET_TYPE_LENGTH = 4; //Warning: Only change in concert with the getInt(), getShort(), putInt() putShort() methods here as well as the YAMCS mission database's constants!
    //Number of bytes for the packet ID field. Comes from fprime's FwTlmPacketizeIdType in config/FpConfig.h
    private static final int PACKET_ID_LENGTH = 2;  //Warning: Only change in concert with the getInt(), getShort(), putInt() putShort() methods here as well as the YAMCS mission database's constants!
    //Length in bytes of the packet size field in the packet header. 
    private static final int PACKET_SIZE_LENGTH = 4; //Warning: Only change in concert with the getInt() and getShort() methods here!
    //Number of bytes for the time tag field. 
    private static final int TIME_TAG_LENGTH = 11; //Ignored for now
    //Number of bytes for the payload of the packet. 
    //i.e. of what is returned by the 2nd Int (packet size), how much is not payload data
    private static final int PAYLOAD_HEADER_LENGTH = PACKET_TYPE_LENGTH + PACKET_ID_LENGTH + TIME_TAG_LENGTH; 
    //Number of bytes for the packet CRC field. 
    private static final int CRC_LENGTH = 4; // Only change in concert with the getInt() and getShort() method here!
    //Minimum number of bytes in a packet to trigger preprocessing - under that it is impossible to have a complete packet
    //Note: incomplete packets are still possible for packets with arguments, do check CRC
    private static final int MIN_PACKET_LENGTH = SYNC_LENGTH+PAYLOAD_HEADER_LENGTH+CRC_LENGTH;
    //Endianness of what is received
    private static final ByteOrder ENDIANNESS = ByteOrder.BIG_ENDIAN;

    // Arbitrary "sequence" count, only to distinguish archived packets as in packets that have the same sequence count are ignored
    // Therefore the max count drives, for a known input frequency, the time to archive
    private AtomicInteger seqCount = new AtomicInteger();
    private static final int MAX_SEQ_COUNT = 0x0000ffff;

    public FprimeTlmPacketPreprocessor(String yamcsInstance) {
        super(yamcsInstance, YConfiguration.emptyConfig());
    }

    /**
     * Processes incoming packets by stripping them of header and trailer to uncover the actually used data.  
     * Refer to YAMCS mission databases to see visually what they uncover (except for events, see event decoder service). Called by YAMCS autonomously 
     * when a packet comes in, after thie preprocessor has been attached to an incoming stream. 
     * 
     * @param pc the packet to process
     * @return the packet to send for interpretation against yamcs mission databases (or by event service)
     */
    @Override
    public TmPacket process(TmPacket tmPacket) {
        byte[] packet = tmPacket.getPacket();

        //Debug
        //log.warn("Received packet of size {}: {}", packet.length, packet);
        
        //If the packet cannot be complete even for packets with no payload, do not trigger preprocessing - keep accumulating
        if (packet.length < MIN_PACKET_LENGTH) {
            log.warn("Short packet received"); //Not sure if the event is going to appear in the log window
            eventProducer.sendWarning("SHORT_PACKET",
                    "Short packet received, length: " + packet.length + "; minimum required length is "+MIN_PACKET_LENGTH+" bytes");
            return null; 
        }

        //Create a convenient byte buffer on the byte stream
        ByteBuffer bb = ByteBuffer.wrap(packet);
        bb.order(ENDIANNESS);  

        // Check start word, if it is not correct drop the preprocessing
        if (bb.getInt() != SYNC_WORD) {
            log.warn("Invalid sync word received"); //Not sure if the event is going to appear in the log window
            eventProducer.sendWarning("INVALID_SYNC",
                    "Invalid sync word received");
            return null; 
        }

        int packetSize = bb.getInt(); //"packet size" as declared by the packet itself, part of the header (see Fprime protocol)
        int packetTypeId = bb.getInt();
        short packetId = bb.getShort();
        // Skip 11 bytes of time tag
        bb.position(bb.position() + TIME_TAG_LENGTH);
        // Extract actual packet data
        byte[] tmData = new byte[packetSize-PAYLOAD_HEADER_LENGTH];
        bb.get(tmData);

        //Debug
        log.warn("Packet type: {}, ID: {}, payload: {}", packet.length, packetTypeId, packetId, tmData);

        // TODO: check checksum

        // Create new TmPacket with extracted data
        ByteBuffer newBuffer = ByteBuffer.allocate(PACKET_TYPE_LENGTH + PACKET_ID_LENGTH + tmData.length);
        newBuffer.putInt(packetTypeId);
        newBuffer.putShort(packetId);
        newBuffer.put(tmData);
        newBuffer.flip(); //Flip double buffer
        TmPacket newPacket = new TmPacket(TimeEncoding.getWallclockTime(), newBuffer.array()); //Receptiontime
        newPacket.setGenerationTime(TimeEncoding.getWallclockTime()); //TODO: take this from the packet header
        newPacket.setSequenceCount(seqCount.getAndIncrement()); //see comments on sequencecount

        return newPacket;
    }

    /**
     * Returns a sequence count for identifying a packet in addition to the generation time.
     * 
     * No sequence counter is available in packet header, so the default implementation uses a local rotating counter.
     */
    protected int getSequenceCount(TmPacket packet) { 
        return seqCount.accumulateAndGet(1, (value, inc) -> {
            var newValue = value + inc;
            if (newValue > MAX_SEQ_COUNT) {
                return 0;
            } else {
                return newValue;
            }
        });
    }
}
