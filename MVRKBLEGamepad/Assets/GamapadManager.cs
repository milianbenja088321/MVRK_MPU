using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GamapadManager : MonoBehaviour
{
    GamepadControls controls;

    public float pitch, yaw;
    Vector2 rotate;
    public bool isPressed;
    private void Awake()
    {
        controls = new GamepadControls();

        controls.Gameplay.Trigger.performed += ctx => TriggerPressed();
        controls.Gameplay.Trigger.canceled += ctx => TriggerReleased();

        controls.Gameplay.Rotation.performed += ctx => rotate = ctx.ReadValue<Vector2>();

    }
    // Update is called once per frame
    void Update()
    {

        pitch = rotate.x;
        yaw = rotate.y;

        //if (rotate.x > 0 && rotate.x < .71)
        //{
        //    Vector2 m = new Vector2(rotate.x, 0) * 50 * Time.deltaTime;
        //    transform.Rotate(m, Space.World);
        //}
        //if (rotate.x < -.3 && rotate.x > -.71)
        //{
        //    Vector2 m = new Vector2(rotate.x, 0) * 50 * Time.deltaTime;
        //    transform.Rotate(m, Space.World);
        //}

    }

    public float GetPitch()
    {
        return pitch;
    }

    public float GetYaw()
    {
        return yaw;
    }

    public void TriggerPressed()
    {
        transform.localScale *= 1.1f;
        //return true;
    }

    public bool TriggerReleased()
    {
        return false;
    }

    private void OnEnable()
    {
        controls.Gameplay.Enable();
    }

    private void OnDisable()
    {
        controls.Gameplay.Disable();
    }
}
