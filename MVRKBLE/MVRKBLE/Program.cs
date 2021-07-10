using System;
using System.Collections.Generic;
using System.IO.MemoryMappedFiles;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Timers;



namespace MVRKBLE
{
    internal class Program
    {
        public static MemoryMappedViewAccessor mmva;
        private static System.Timers.Timer timer1;
        private static SensorModule sensormodule;
        private static bool actflg = false;
        static void Main(string[] args)
        {

            try
            {
                Program.mmva = MemoryMappedFile.CreateNew("UnityEsp32Ble", 1024L).CreateViewAccessor();
            }
            catch (Exception ex)
            {
                Console.WriteLine("Exception: " + ex.Message);
            }
            byte structure = 0;
            Program.mmva.Write<byte>(0L, ref structure);
            Program.mmva.Write<byte>(100L, ref structure);
            Thread.Sleep(1000);
            Program.timer1 = new System.Timers.Timer();
            Program.timer1.Interval = 10000.0;
            Program.timer1.Elapsed += new ElapsedEventHandler(Program.Timer1_Elapsed);
            Program.timer1.Enabled = true;
            Program.sensormodule = new SensorModule();
            Program.sensormodule.Start();
        }

        public static void Send(byte[] report)
        {
            Program.actflg = true;
            byte length = (byte)report.Length;
            Program.mmva.Write<byte>(0L, ref length);
            Program.mmva.WriteArray<byte>(1L, report, 0, report.Length);
        }

        private static void Timer1_Elapsed(object sender, ElapsedEventArgs e)
        {
            Console.WriteLine(string.Format("Called Timer1_Elapsed", (object[])Array.Empty<object>()));
            if (Program.actflg)
            {
                Program.actflg = false;
            }
            else
            {
                Program.sensormodule.advWatcher.Stop();
                Program.mmva.Dispose();
                Environment.Exit(0);
            }
        }
    }
}