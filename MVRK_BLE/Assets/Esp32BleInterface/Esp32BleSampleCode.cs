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
    [SerializeField] GameObject firstObject, secondObject;

    float[] sOne = new float[3];
    float[] sTwo = new float[3];
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

        float[] boardData = new float[8];
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
        sOne[0] = float.Parse(arr[0]);
        sOne[1] = float.Parse(arr[1]);
        sOne[2] = float.Parse(arr[2]);
        sTwo[0] = float.Parse(arr[3]);
        sTwo[1] = float.Parse(arr[4]);
        sTwo[2] = float.Parse(arr[5]);
        aState = (int)float.Parse(arr[6]);
        bState = (int)float.Parse(arr[7]);


        if(aState == 1)
        {
            testMaterial.color = Color.white;
        }

        if(bState == 1)
        {
            testMaterial.color = Color.blue;
        }

        firstObject.transform.rotation = Quaternion.AngleAxis(sOne[1], Vector3.up) * Quaternion.AngleAxis(sOne[0], Vector3.right) * Quaternion.AngleAxis(sOne[2], Vector3.forward);
        secondObject.transform.rotation = Quaternion.AngleAxis(sTwo[1], Vector3.up) * Quaternion.AngleAxis(sTwo[0], Vector3.right) * Quaternion.AngleAxis(sTwo[2], Vector3.forward);
    }
    
    private void OnApplicationQuit()
    {
        UnityEngine.Debug.LogWarning("OnApplicationQuit");
        m_Esp32BleLib.Quit();
    }
}
