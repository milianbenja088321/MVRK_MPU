using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BLEManager : MonoBehaviour
{
    PlayerControls controls;
   Quaternion rot;
    Vector3 move;

    public float x, y, z;
    private void Awake()
    {
        controls = new PlayerControls();

        controls.Gameplay.Rot.performed += ctx => move = ctx.ReadValue<Vector3>();
        controls.Gameplay.Rot.canceled += ctx => move = Vector3.zero;
        
    }
    // Start is called before the first frame update
    void Start()
    {
        rot = transform.rotation;
    }

    private void OnEnable()
    {
        controls.Gameplay.Enable();
    }

    private void OnDisable()
    {
        controls.Gameplay.Disable();
    }
    // Update is called once per frame
    void Update()
    {
        x = move.x;
        y = move.y;
        z = move.z;

        Vector3 r = new Vector3(x, y, z);
        transform.Rotate(r);
    }
}
