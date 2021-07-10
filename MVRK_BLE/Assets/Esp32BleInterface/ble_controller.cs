using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System.Text;

public class btle_controller : MonoBehaviour
{

    // ----------------------------------------------------------------- 
    // change these to match the bluetooth device you're connecting to: 
    // ----------------------------------------------------------------- 
    // private string _FullUID = "713d****-503e-4c75-ba94-3148f18d941e"; // redbear module pattern 
    private string _FullUID = "0000****-0000-1000-8000-00805f9b34fb";     // BLE-CC41a module pattern 
    private string _serviceUUID = "ffe0";
    private string _readCharacteristicUUID = "ffe1";
    private string _writeCharacteristicUUID = "ffe1";
    private string deviceToConnectTo = "ChrisBLE";

    public bool isConnected = false;
    private bool _readFound = false;
    private bool _writeFound = false;
    private string _connectedID = null;

    private Dictionary<string, string> _peripheralList;
    private float _subscribingTimeout = 0f;

    public Text txtDebug;
    public GameObject uiPanel;
    public Text txtSend;
    public Text txtReceive;
    public Button btnSend;


    // Use this for initialization 
   public  void Start()
    {

    }

    // Update is called once per frame 
    void Update()
    {

    }

}