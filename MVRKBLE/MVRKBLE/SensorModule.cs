using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Threading;
using Windows.Devices.Bluetooth;
using Windows.Devices.Bluetooth.Advertisement;
using Windows.Devices.Bluetooth.GenericAttributeProfile;
using Windows.Foundation;
using Windows.Storage.Streams;

namespace MVRKBLE
{
    internal class SensorModule
    {
        private const string SENSOR_NAME = "Esp32Ble";
        private static Guid UUID_TEMP_SERV = new Guid("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
        private static Guid UUID_TEMP_DATA = new Guid("beb5483e-36e1-4688-b7f5-ea07361b26a8");
        public BluetoothLEAdvertisementWatcher advWatcher;

        public void Start()
        {
            Console.WriteLine(nameof(Start));
            this.advWatcher = new BluetoothLEAdvertisementWatcher();
            this.advWatcher.ScanningMode((BluetoothLEScanningMode)0);
            BluetoothLEAdvertisementWatcher advWatcher = this.advWatcher;
            // ISSUE: method pointer
            WindowsRuntimeMarshal.AddEventHandler<TypedEventHandler<BluetoothLEAdvertisementWatcher, BluetoothLEAdvertisementReceivedEventArgs>>((Func<M0, EventRegistrationToken>)new Func<TypedEventHandler<BluetoothLEAdvertisementWatcher, BluetoothLEAdvertisementReceivedEventArgs>, EventRegistrationToken>(advWatcher.add_Received), new Action<EventRegistrationToken>(advWatcher.remove_Received), (M0)new TypedEventHandler<BluetoothLEAdvertisementWatcher, BluetoothLEAdvertisementReceivedEventArgs>((object)this, __methodptr(Watcher_Received)));
            this.advWatcher.Start();
            while (true)
            {
                byte structure;
                Program.mmva.Read<byte>(100L, out structure);
                if (structure != (byte)5)
                    Thread.Sleep(500);
                else
                    break;
            }
            this.advWatcher.Stop();
            Program.mmva.Dispose();
            Console.WriteLine("Stop");
        }

        private void Watcher_Received(
          BluetoothLEAdvertisementWatcher sender,
          BluetoothLEAdvertisementReceivedEventArgs args)
        {
            this.CheckArgs(args);
        }

        public async void CheckArgs(BluetoothLEAdvertisementReceivedEventArgs args)
        {
            Console.WriteLine("---Received---");
            TaskAwaiter<BluetoothLEDevice> awaiter1 = (TaskAwaiter<BluetoothLEDevice>)WindowsRuntimeSystemExtensions.GetAwaiter<BluetoothLEDevice>((IAsyncOperation<M0>)BluetoothLEDevice.FromBluetoothAddressAsync(args.BluetoothAddress));
            TaskAwaiter<BluetoothLEDevice> taskAwaiter1;
            int num;
            if (!((TaskAwaiter<BluetoothLEDevice>)ref awaiter1).IsCompleted)
            {
                // ISSUE: reference to a compiler-generated field
                this.\u003C\u003E1__state = num = 0;
                taskAwaiter1 = awaiter1;
                // ISSUE: variable of a compiler-generated type
                SensorModule.\u003CCheckArgs\u003Ed__6 checkArgsD6 = this;
                // ISSUE: reference to a compiler-generated field
                // ISSUE: cast to a reference type
                // ISSUE: cast to a reference type
                ((AsyncVoidMethodBuilder)ref this.\u003C\u003Et__builder).AwaitUnsafeOnCompleted < TaskAwaiter<BluetoothLEDevice>, SensorModule.\u003CCheckArgs\u003Ed__6 > ((M0 &) ref awaiter1, (M1 &) ref checkArgsD6);
            }
            else
            {
                BluetoothLEDevice dev = ((TaskAwaiter<BluetoothLEDevice>)ref awaiter1).GetResult();
                bool find = false;
                IList<Guid> bleServiceUUIDs = args.Advertisement.ServiceUuids;
                foreach (Guid guid in (IEnumerable<Guid>)bleServiceUUIDs)
                {
                    Guid uuidone = guid;
                    if (uuidone == SensorModule.UUID_TEMP_SERV)
                    {
                        find = true;
                        break;
                    }
                    uuidone = new Guid();
                }
                if (!(dev.Name == "Esp32Ble"))
                    return;
                try
                {
                    Console.WriteLine("Service Find！");
                    this.advWatcher.Stop();
                    TaskAwaiter<BluetoothLEDevice> awaiter2 = (TaskAwaiter<BluetoothLEDevice>)WindowsRuntimeSystemExtensions.GetAwaiter<BluetoothLEDevice>((IAsyncOperation<M0>)BluetoothLEDevice.FromBluetoothAddressAsync(args.BluetoothAddress));
                    if (!((TaskAwaiter<BluetoothLEDevice>)ref awaiter2).IsCompleted)
                    {
                        // ISSUE: reference to a compiler-generated field
                        this.\u003C\u003E1__state = num = 1;
                        taskAwaiter1 = awaiter2;
                        // ISSUE: variable of a compiler-generated type
                        SensorModule.\u003CCheckArgs\u003Ed__6 checkArgsD6 = this;
                        // ISSUE: reference to a compiler-generated field
                        // ISSUE: cast to a reference type
                        // ISSUE: cast to a reference type
                        ((AsyncVoidMethodBuilder)ref this.\u003C\u003Et__builder).AwaitUnsafeOnCompleted < TaskAwaiter<BluetoothLEDevice>, SensorModule.\u003CCheckArgs\u003Ed__6 > ((M0 &) ref awaiter2, (M1 &) ref checkArgsD6);
                    }
                    else
                    {
                        BluetoothLEDevice device = ((TaskAwaiter<BluetoothLEDevice>)ref awaiter2).GetResult();
                        TaskAwaiter<GattDeviceServicesResult> awaiter3 = (TaskAwaiter<GattDeviceServicesResult>)WindowsRuntimeSystemExtensions.GetAwaiter<GattDeviceServicesResult>((IAsyncOperation<M0>)device.GetGattServicesForUuidAsync(SensorModule.UUID_TEMP_SERV));
                        if (!((TaskAwaiter<GattDeviceServicesResult>)ref awaiter3).IsCompleted)
                        {
                            // ISSUE: reference to a compiler-generated field
                            this.\u003C\u003E1__state = num = 2;
                            TaskAwaiter<GattDeviceServicesResult> taskAwaiter2 = awaiter3;
                            // ISSUE: variable of a compiler-generated type
                            SensorModule.\u003CCheckArgs\u003Ed__6 checkArgsD6 = this;
                            // ISSUE: reference to a compiler-generated field
                            // ISSUE: cast to a reference type
                            // ISSUE: cast to a reference type
                            ((AsyncVoidMethodBuilder)ref this.\u003C\u003Et__builder).AwaitUnsafeOnCompleted < TaskAwaiter<GattDeviceServicesResult>, SensorModule.\u003CCheckArgs\u003Ed__6 > ((M0 &) ref awaiter3, (M1 &) ref checkArgsD6);
                        }
                        else
                        {
                            GattDeviceServicesResult services = ((TaskAwaiter<GattDeviceServicesResult>)ref awaiter3).GetResult();
                            TaskAwaiter<GattCharacteristicsResult> awaiter4 = (TaskAwaiter<GattCharacteristicsResult>)WindowsRuntimeSystemExtensions.GetAwaiter<GattCharacteristicsResult>((IAsyncOperation<M0>)services.Services[0].GetCharacteristicsForUuidAsync(SensorModule.UUID_TEMP_DATA));
                            if (!((TaskAwaiter<GattCharacteristicsResult>)ref awaiter4).IsCompleted)
                            {
                                // ISSUE: reference to a compiler-generated field
                                this.\u003C\u003E1__state = num = 3;
                                TaskAwaiter<GattCharacteristicsResult> taskAwaiter2 = awaiter4;
                                // ISSUE: variable of a compiler-generated type
                                SensorModule.\u003CCheckArgs\u003Ed__6 checkArgsD6 = this;
                                // ISSUE: reference to a compiler-generated field
                                // ISSUE: cast to a reference type
                                // ISSUE: cast to a reference type
                                ((AsyncVoidMethodBuilder)ref this.\u003C\u003Et__builder).AwaitUnsafeOnCompleted < TaskAwaiter<GattCharacteristicsResult>, SensorModule.\u003CCheckArgs\u003Ed__6 > ((M0 &) ref awaiter4, (M1 &) ref checkArgsD6);
                            }
                            else
                            {
                                GattCharacteristicsResult characteristics = ((TaskAwaiter<GattCharacteristicsResult>)ref awaiter4).GetResult();
                                if (characteristics.Status == 0)
                                {
                                    GattCharacteristic gattCharacteristic = ((IEnumerable<GattCharacteristic>)characteristics.Characteristics).First<GattCharacteristic>();
                                    TaskAwaiter<GattCommunicationStatus> awaiter5 = (TaskAwaiter<GattCommunicationStatus>)WindowsRuntimeSystemExtensions.GetAwaiter<GattCommunicationStatus>((IAsyncOperation<M0>)gattCharacteristic.WriteClientCharacteristicConfigurationDescriptorAsync((GattClientCharacteristicConfigurationDescriptorValue)1));
                                    if (!((TaskAwaiter<GattCommunicationStatus>)ref awaiter5).IsCompleted)
                                    {
                                        // ISSUE: reference to a compiler-generated field
                                        this.\u003C\u003E1__state = num = 4;
                                        TaskAwaiter<GattCommunicationStatus> taskAwaiter2 = awaiter5;
                                        // ISSUE: variable of a compiler-generated type
                                        SensorModule.\u003CCheckArgs\u003Ed__6 checkArgsD6 = this;
                                        // ISSUE: reference to a compiler-generated field
                                        // ISSUE: cast to a reference type
                                        // ISSUE: cast to a reference type
                                        ((AsyncVoidMethodBuilder)ref this.\u003C\u003Et__builder).AwaitUnsafeOnCompleted < TaskAwaiter<GattCommunicationStatus>, SensorModule.\u003CCheckArgs\u003Ed__6 > ((M0 &) ref awaiter5, (M1 &) ref checkArgsD6);
                                        return;
                                    }
                                    GattCommunicationStatus status = ((TaskAwaiter<GattCommunicationStatus>)ref awaiter5).GetResult();
                                    if (status == 0)
                                    {
                                        GattCharacteristic gattCharacteristic1 = gattCharacteristic;
                                        // ISSUE: method pointer
                                        WindowsRuntimeMarshal.AddEventHandler<TypedEventHandler<GattCharacteristic, GattValueChangedEventArgs>>((Func<M0, EventRegistrationToken>)new Func<TypedEventHandler<GattCharacteristic, GattValueChangedEventArgs>, EventRegistrationToken>(gattCharacteristic1.add_ValueChanged), new Action<EventRegistrationToken>(gattCharacteristic1.remove_ValueChanged), (M0)new TypedEventHandler<GattCharacteristic, GattValueChangedEventArgs>((object)this, __methodptr(Changed_data)));
                                    }
                                    gattCharacteristic = (GattCharacteristic)null;
                                }
                                device = (BluetoothLEDevice)null;
                                services = (GattDeviceServicesResult)null;
                                characteristics = (GattCharacteristicsResult)null;
                            }
                        }
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine("Exception..." + ex.Message + ")");
                }
            }
        }

        public void Changed_data(GattCharacteristic sender, GattValueChangedEventArgs eventArgs)
        {
            Console.WriteLine(string.Format("characteristicChanged...Length={0}", (object)eventArgs.CharacteristicValue.Length));
            byte[] report = new byte[(int)eventArgs.CharacteristicValue.Length];
            DataReader.FromBuffer(eventArgs.CharacteristicValue).ReadBytes(report);
            Console.WriteLine(string.Format("characteristicChanged...{0} ", (object)report) + report[0].ToString());
            Program.Send(report);
        }
    }
}
