using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace serialControl1
{
    class SerialCn
    {
        private SerialPort serialPort;

        public SerialCn()
        {
            serialPort = new SerialPort();
        }

        public bool isOpen()
        {
            if (serialPort.IsOpen)
                return true;
            else return false;
        }

        public void serialConnect(string portName, int baudRate)
        {
            serialPort.PortName = portName;
            serialPort.BaudRate = baudRate;

            serialPort.Open();
        }

        public void serialDisconnect()
        {
            serialPort.Close();
        }

        public void sendSerialMsg(String msg)
        {
            serialPort.Write(msg);
        }
    }
}
