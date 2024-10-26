#include "isobus/hardware_integration/nuvoton_plugin.hpp"
#include "isobus/isobus/can_constants.hpp"
#include "isobus/isobus/can_stack_logger.hpp"
#include "isobus/utility/system_timing.hpp"
#include "isobus/utility/to_string.hpp"

uint32_t idrRec = 0;
bool msgReceived = false;
bool CAN_status = true;

void CAN0_IRQHandler(void)
{
    uint32_t u8IIDRstatus;

    u8IIDRstatus = CAN0->IIDR;

    if(u8IIDRstatus == 0x00008000)        /* Check Status Interrupt Flag (Error status Int and Status change Int) */
    {
        /**************************/
        /* Status Change interrupt*/
        /**************************/
        if(CAN0->STATUS & CAN_STATUS_RXOK_Msk)
        {
            CAN0->STATUS &= ~CAN_STATUS_RXOK_Msk;   /* Clear Rx Ok status*/

            //("RX OK INT\n") ;
        }

        if(CAN0->STATUS & CAN_STATUS_TXOK_Msk)
        {
            CAN0->STATUS &= ~CAN_STATUS_TXOK_Msk;    /* Clear Tx Ok status*/

            //printf("TX OK INT\n") ;
        }

        /**************************/
        /* Error Status interrupt */
        /**************************/
        if(CAN0->STATUS & CAN_STATUS_EWARN_Msk)
        {
            //printf("EWARN INT\n") ;
        }

        if(CAN0->STATUS & CAN_STATUS_BOFF_Msk)
        {
            CAN_status = false;
        }
    }
    else if(u8IIDRstatus != 0)
    {
        msgReceived = true;
        idrRec = u8IIDRstatus;
    }


}

namespace isobus
{
	NuPlugin::NuPlugin() :
	{
	}

	NuPlugin::~NuPlugin()
	{
		close();
	}

	bool NuPlugin::get_is_valid() const
	{
		return  CAN_status;
	}

	void NuPlugin::close()
	{
		  /* Disable CAN0 Clock and Reset it */
        SYS->IPRST1 |= SYS_IPRST1_CAN0RST_Msk;
        SYS->IPRST1 &= ~SYS_IPRST1_CAN0RST_Msk;
        CLK->APBCLK0 &= ~CLK_APBCLK0_CAN0CKEN_Msk;
	}

	void NuPlugin::open()
	{
        /** 
         * init uart for debug
         */
        DEBUG_PORT_Init();

		 // Enable IP clock
        CLK->APBCLK0 |= CLK_APBCLK0_CAN0CKEN_Msk;

        // Reset CAN0
        SYS->IPRST1 |= SYS_IPRST1_CAN0RST_Msk;
        SYS->IPRST1 &= ~SYS_IPRST1_CAN0RST_Msk;

        /** Enable Can Module CAN0 and 250 mbit for isobus compability */
        CAN_Open(CAN0,  250000, CAN_BASIC_MODE);

        CAN_EnableInt(CAN0, CAN_CON_IE_Msk | CAN_CON_SIE_Msk);
        NVIC_SetPriority(CAN0_IRQn, (1 << __NVIC_PRIO_BITS) - 2);
        NVIC_EnableIRQ(CAN0_IRQn);
	}

	bool NuPlugin::read_frame(isobus::CANMessageFrame &canFrame)
	{
        bool retVal = false;

        if(msgReceived){

            STR_CANMSG_T rmsg = {};

            CAN_Receive(CAN0, idrRec, &rmsg);

            if(rmsg.FrameType == CAN_DATA_FRAME){
                canFrame.identifier = rmsg.Id;
				canFrame.isExtendedFrame = rmsg.IdType;
				canFrame.dataLength = rmsg.DLC;
				if (isobus::CAN_DATA_LENGTH >= canFrame.dataLength)
				{
					memset(canFrame.data, 0, sizeof(canFrame.data));
					memcpy(canFrame.data, rmsg.Data, canFrame.dataLength);
					retVal = true;
				}
            }
        }

		return retVal;
	}

    void NuPlugin::DEBUG_PORT_Init()
    {
        /*---------------------------------------------------------------------------------------------------------*/
        /* Init UART                115200                                                                               */
        /*---------------------------------------------------------------------------------------------------------*/
        DEBUG_PORT->LINE = UART_PARITY_NONE | UART_STOP_BIT_1 | UART_WORD_LEN_8;
        DEBUG_PORT->BAUD = UART_BAUD_MODE2 | UART_BAUD_MODE2_DIVIDER(__HIRC, 115200);

    }

	bool NuPlugin::write_frame(const isobus::CANMessageFrame &canFrame)
	{

		STR_CANMSG_T tmsg = {};

		rmsg.Id = canFrame.identifier;
		rmsg.IdType = canFrame.isExtendedFrame;
		rmsg.DLC = canFrame.dataLength;
		memcpy(message.data, rmsg.Data, canFrame.dataLength);

		return CAN_Transmit(CAN0, 0, &msg1);
	}
}