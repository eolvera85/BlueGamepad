#include "rbc_switch_spi.h"

/**
 * @brief Reads a chunk of memory from SPI (emulated)
 * @param[in] offset_address The address segment to read from.
 * @param[in] address The actual memory address to read from the segment.
 * @param[in] length The amount of bytes to pull from SPI emulated
 */
void ns_spi_readfromaddress(uint8_t offset_address, uint8_t address, uint8_t length)
{
    ns_input_report[15] = address;
    ns_input_report[16] = offset_address;
    ns_input_report[19] = length;

    ns_input_report_size += 5;

    uint8_t output_spi_data[30] = {};

    for (int i = 0; i < length; i++)
    {
        output_spi_data[i] = ns_spi_getaddressdata(offset_address, address+i);
    }

    // Add to the input report size to ensure it matches the SPI output.
    ns_input_report_size += length;

    // 
    // Do a bulk set for the input report
    ns_report_bulkset(SPI_READ_OUTPUT_IDX, output_spi_data, length);
}

/**
 * @brief Handles SPI reads of misc data
 * or unknown addresses :)
 */
uint8_t ns_spi_getaddressdata(uint8_t offset_address, uint8_t address)
{
    switch (offset_address)
    {
        // Patch ROM not needed
        case 0x00:
            
            return 0x00;
            break;

        // Failsafe mechanism not needed
        case 0x10:
            
            return 0x00;
            break;

        // Pairing data
        case 0x20 ... 0x40:
            switch (address)
            {   
                // Magic number determines if Pairing info is used
                case 0x00:
                    return 0x00; // 0x95 if used
                    break;

                // Size of pairing data
                case 0x01:
                    return 0x22;
                    break;

                // Checksum
                case 0x02:
                case 0x03:
                    return 0x00;
                    break;

                // Host BT address (Big-endian)
                case 0x04 ... 0x09:
                    return loaded_settings.ns_host_bt_address[address-4];
                    break;

                // Bluetooth LTK (Little-endian) NOT IMPLEMENTED YET
                case 0x0A ... 0x19:
                    return 0x00;
                    break;

                // Host capability 0x68 is Nintendo Switch. 0x08 is PC
                case 0x24:
                    return 0x68;
                    break;

                // Pairing section 2. Respond all 0xFF.
                case 0x26 ... 0x4B:
                    return 0xFF;
                    break;
                
                // No match... Return 0
                default:
                    return 0x00;
                    break;
            }
            break;

        // Shipment info. ALWAYS return 0.
        case 0x50:
            return 0x00;
            break;

        // Factory configuration and calibration
        case 0x60:
            switch (address)
            {   
                // Serial number disabled. First byte >= 0x80 means NO serial.
                case 0x00:
                    return 0x81;
                    break;
                // Return remaining serial number bytes as 0.
                case 0x01 ... 0x0F:
                    return 0x00;
                    break;

                // Return factory controller type
                case 0x12:
                    return ns_controller_data.controller_type_primary;
                    break;

                // Unknown constant
                case 0x13:
                    return ns_controller_data.controller_type_secondary;
                    //return 0xA0;
                    break;

                // Returns a bool indicating if color is set.
                case 0x1B:
                    return NS_COLOR_SET;
                    break;

                // TO-DO - Implement factory 6-Axis calibration.
                case 0x20 ... 0x37:
                    return 0x00;
                    break;

                // TO-DO - Implement factory left stick calibration.
                case 0x3D ... 0x45:
                    return ns_input_stickcaldata.l_stick_cal[address-0x3B];
                    break;

                // TO-DO - Implement factory right stick calibration.
                case 0x46 ... 0x4E:
                    return ns_input_stickcaldata.l_stick_cal[address-0x44];
                    break;

                // TO-DO - Implement factory body color.
                case 0x50:
                    return 0x00; //loaded_settings.color_r;
                    break;
                case 0x51:
                    return 0x00; //loaded_settings.color_g;
                    break;
                case 0x52:
                    return 0x00; //loaded_settings.color_b;
                    break;

                // TO-DO - Implement factory buttons color.
                case 0x53:
                    return 0xCD; //0x00;
                    break;
                case 0x54:
                    return 0xCD; //0x00;
                    break;                    
                case 0x55:
                    return 0xCD; //0x00;
                    break;

                // TO-DO - Implement factory left grip color.
                case 0x56:
                    return 0x46; //0x00;
                    break;
                case 0x57:
                    return 0xB8; //0x00;
                    break;
                case 0x58:
                    return 0xD3; //0x00;
                    break;

                // TO-DO - Implement factory right grip color.
                case 0x59:
                    return 0xEF; //0x00;
                    break;
                case 0x5A:
                    return 0x5C; //0x00;
                    break;
                case 0x5B:
                    return 0x52; //0x00;
                    break;

                // SNES 0x00 American 0x02 Europe
                case 0x5C:
                    return ns_snes_controller_type;
                    break;

                default:
                    return 0x00;
                    break;
            }
            break;

        // User calibration values
        case 0x80:
            switch (address)
            {

                // User left stick calibration data
                case 0x10 ... 0x1A:
                    return ns_input_stickcaldata.l_stick_cal[address-0x10];
                    break;

                // TO-DO - User right stick calibration data
                case 0x1B ... 0x25:
                    return ns_input_stickcaldata.l_stick_cal[address-0x1B];
                    break;

                // TO-DO - User 6-axis calibration data Magic bytes
                case 0x26:
                case 0x27:
                    return 0x00;
                    break;

                // TO-DO - User 6-axis calibration data
                case 0x28 ... 0x3F:
                    return 0x00;
                    break;

                default:
                    return 0x00;
                    break;
            }
            break;

        default:
            return 0x00;
            break;
    }

    return 0x00;
}