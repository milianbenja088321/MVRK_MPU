using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Esp32BleSampleCode : MonoBehaviour
{
    public float accelx;
    public float accely;
    public float accelz;

    public int aState, bState = 0;
    
    private Esp32BleLib m_Esp32BleLib;
    [SerializeField] private Material testMaterial;

    public btle_controller bob;
    void Start()
    {

        //this.transform.localScale = new Vector3(2, 2, 2);

        UnityEngine.Debug.LogWarning("start");
        m_Esp32BleLib = gameObject.AddComponent<Esp32BleLib>();
        m_Esp32BleLib.Esp32BleStart();

    }

    // Update is called once per frame
    void Update()
    {
        string strbuf;

        float[] boardData = new float[5];
        byte[] data;

        //UnityEngine.Debug.LogWarning("Update");
        data = m_Esp32BleLib.UpdateRead();
        if (data.Length == 0)
        {
            return;
        }
        strbuf = " Update:";
        for (int i = 0; i < data.Length; i++)
        {
            strbuf += " " + data[i];

        }
        UnityEngine.Debug.LogWarning(data.Length);

        string text = System.Text.Encoding.UTF8.GetString(data);
        UnityEngine.Debug.LogWarning(" Read: " + text);
        string[] arr = text.Split(',');
        boardData[0] = float.Parse(arr[0]);
        boardData[1] = float.Parse(arr[1]);
        boardData[2] = float.Parse(arr[2]);
        boardData[3] = float.Parse(arr[3]);
        boardData[4] = float.Parse(arr[4]);


        accelz = boardData[0] * -1;
        accely = boardData[1];
        accelx = boardData[2] * -1;

        aState = (int)boardData[3];
        bState = (int)boardData[4];

        if(aState == 1)
        {
            testMaterial.color = Color.white;
        }

        if(bState == 1)
        {
            testMaterial.color = Color.blue;
        }

        transform.rotation = Quaternion.AngleAxis(accelx, Vector3.up) * Quaternion.AngleAxis(accely, Vector3.right) * Quaternion.AngleAxis(accelz, Vector3.forward);
    }
    
    private void OnApplicationQuit()
    {
        UnityEngine.Debug.LogWarning("OnApplicationQuit");
        m_Esp32BleLib.Quit();
    }
}
