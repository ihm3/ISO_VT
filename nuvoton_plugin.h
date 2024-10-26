#ifndef __NU_PLUGIN__
#define __NU_PLUGIN__

#include "isobus/hardware_integration/can_hardware_plugin.hpp"
#include "isobus/isobus/can_hardware_abstraction.hpp"
#include "isobus/isobus/can_message_frame.hpp"


#include "NuMicro.h"

namespace isobus
{
    //================================================================================================
	/// @class NU plugin
	///
	/// @brief A driver for  Nuvoton uC s -- M263
	//================================================================================================
    class NuPlugin : public CANHardwarePlugin
	{
	public:
		
		explicit NuPlugin(CAN_T* tCan);

		virtual ~NuPlugin();

		/// @brief Returns if the socket connection is valid
		/// @returns `true` if connected, `false` if not connected
		bool get_is_valid() const override;

		/// @brief Closes the socket
		void close() override;

		/// @brief Connects to the socket
		void open() override;

		/// @brief Returns a frame from the hardware (synchronous), or `false` if no frame can be read.
		/// @param[in, out] canFrame The CAN frame that was read
		/// @returns `true` if a CAN frame was read, otherwise `false`
		bool read_frame(isobus::CANMessageFrame &canFrame) override;

		/// @brief Writes a frame to the bus (synchronous)
		/// @param[in] canFrame The frame to write to the bus
		/// @returns `true` if the frame was written, otherwise `false`
		bool write_frame(const isobus::CANMessageFrame &canFrame) override;

        private:

        void DEBUG_PORT_Init();

        STR_CANMSG_T rrMsg;

	};

}




#endif // nu plugin