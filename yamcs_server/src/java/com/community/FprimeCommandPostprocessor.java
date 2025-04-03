package com.community;

import java.nio.ByteBuffer;
import java.util.zip.CRC32;

import org.yamcs.YConfiguration;
import org.yamcs.commanding.PreparedCommand;
import org.yamcs.tctm.CommandPostprocessor;

/**
 * FprimeCommandPostprocessor is the class which packetises commands previously structured according to the mission database, using the Fprime command packet protocol
 * so that these packets can then be routed to the F' deframer as-is. 
 * 
 * @author GM
 **/
public class FprimeCommandPostprocessor implements CommandPostprocessor {    
    //Start word for the Fprime packet protocol
    private static final byte[] START_WORD = { (byte) 0xDE, (byte) 0xAD, (byte) 0xBE, (byte) 0xEF };
    //Number of bytes for the packet "size" or "length" field. c.f. FprimeTlmPacketPreprocessor
    private static final int N_BYTES_LENGTH = 4; //Only change in concert with putInt() putShort() methods here
    //Number of bytes for the packet type field. C.f. FprimeTlmPacketPreprocessor
    private static final int N_BYTES_PACKETTYPE = 4; //Only change in concert with putInt() putShort() methods here
    //Number of bytes for the checksum field. C.f FprimeTlmPacketPreprocessor
    private static final int N_BYTES_CRC = 4; //Only change in concert with putInt() putShort() methods here
    //Packet type number corresponding to commands. C.f FprimeTlmPacketPreprocessor
    private static final int PACKETTYPE = 0; 
    //For the size of the opcode variable, see yamcs_mdb_gen's constant since this is here part of the packet payload

    public FprimeCommandPostprocessor(String yamcsInstance) {
    }

    public FprimeCommandPostprocessor(String yamcsInstance, YConfiguration config) {
    }

    /**
     * Processes a prepared command by adding F' specific header, length,
     * packet type, and CRC32 checksum. 
     * Autonomously called by YAMCS after this post processor is attached to a command stream. 
     * 
     * @param pc the prepared command to process
     * @return byte array containing the processed command, which will be sent out to Fprime
     */
    @Override
    public byte[] process(PreparedCommand pc) {
        byte[] commandData = pc.getBinary();
        
        //Precalculate the length. The length field does not cover start word or checksum
        int length = commandData.length+N_BYTES_PACKETTYPE;
        
        //Note: Alternatively ByteBuffer buffer = ByteBuffer.allocate();
        //Create a new byte array, larger, which will contain the prepared command as well as header and trailer
        byte[] finalArray = new byte[START_WORD.length + N_BYTES_LENGTH + length + N_BYTES_CRC];
        ByteBuffer buffer = ByteBuffer.wrap(finalArray);

        // big-endian order, construct the packet
        buffer.put(START_WORD);
        buffer.putInt(length); 
        buffer.putInt(PACKETTYPE); 
        buffer.put(commandData); //Opcode first which takes the place of packet ID, then potential arguments - see mission database

        // Calculate CRC-32. Fprime uses https://www.lammertbies.nl/comm/info/crc-calculation
        CRC32 crc = new CRC32();
        crc.update(finalArray, 0, finalArray.length - N_BYTES_CRC);
        buffer.putInt((int) crc.getValue());

        return finalArray;
    }
}
