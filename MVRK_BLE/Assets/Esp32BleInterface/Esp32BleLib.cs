using System;
using System.Diagnostics;
using System.IO.MemoryMappedFiles;
using System.Threading;
using UnityEngine;

public class Esp32BleLib : MonoBehaviour
{
    private Process proc;
    private MemoryMappedViewAccessor mmva;

    public void Esp32BleStart()
    {
        UnityEngine.Debug.LogWarning((object)nameof(Esp32BleStart));
        try
        {
            this.proc = new Process();
            this.proc.StartInfo.FileName = "Assets\\Esp32BleInterface\\Esp32BleConnect.exe";
            this.proc.StartInfo.Arguments = "";
            this.proc.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
            this.proc.Start();

            Thread.Sleep(1000);

            this.mmva = MemoryMappedFile.OpenExisting("UnityEsp32Ble").CreateViewAccessor();
        }
        catch (Exception ex)
        {
            UnityEngine.Debug.LogWarning((object)("Exception: " + ex.Message));
        }
    }

    public byte[] UpdateRead()
    {
        byte structure;

        this.mmva.Read<byte>(0L, out structure);

        if (structure == (byte)0)
            return new byte[0];

        byte[] array = new byte[(int)structure];

        this.mmva.ReadArray<byte>(1L, array, 0, array.Length);

        string str = " Read1: " + array.Length.ToString();

        for (int index = 0; index < array.Length; ++index)
            str = str + " " + array[index].ToString();


        return array;
    }

    public void Quit()
    {
        UnityEngine.Debug.LogWarning((object)nameof(Quit));

        byte structure = 5;

        this.mmva.Write<byte>(100L, ref structure);
    }
}
