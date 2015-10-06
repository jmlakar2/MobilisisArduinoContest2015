using serialControl1;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace safe_client
{
    public partial class SafeClient : Form
    {
        SerialPort serialPort = new SerialPort();
        //SerialCn scn = new SerialCn();
        String msg = "";
        public SafeClient()
        {
            InitializeComponent();
            serialPort.DataReceived += new SerialDataReceivedEventHandler(getData);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (serialPort.IsOpen)
            {
                serialPort.Close();
                connBtn.Enabled = true;
                discBtn.Enabled = false;
            }
        }

        private void connBtn_Click(object sender, EventArgs e)
        {
            try
            {
                serialPort.PortName = "COM17";
                serialPort.BaudRate = 9600;
                serialPort.Open();

                connBtn.Enabled = false;
                discBtn.Enabled = true;
            }
            catch
            {
                MessageBox.Show("ARDUINO IS NOT CONNECTED");
            }
        }

        private void getData(object sender, SerialDataReceivedEventArgs e)
        {
            msg = "";
            msg = serialPort.ReadLine();
            this.Invoke(new EventHandler(sendMail));
        }

        private void sendMail(object sender, EventArgs e)
        {
            String sURL = "";
            if (msg.Contains("opened"))
            {
                WebClient webClient = new WebClient();
                webClient.QueryString.Add("action", "opened");
                webClient.DownloadString("http://something/arduino_contest?action=opened");
            }
            else if (msg.Contains("intruder"))
            {
                sURL = "http://something/arduino_contest?action=intruder";
                WebClient webClient = new WebClient();
                webClient.QueryString.Add("action", "intruder");
                webClient.DownloadString("http://something/arduino_contest");
            }
        }
    }
}
