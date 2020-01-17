
using UnityEngine;
[RequireComponent(typeof(Rigidbody))]
public class PlayerMotor : MonoBehaviour
{
    Rigidbody rb;
    [SerializeField] Camera pCam;
    private Vector3 velocity = Vector3.zero;
    private Vector3 rotationPlayer = Vector3.zero;
    private float rotationCamera;
    private float jumpForce = 0;
    [SerializeField] private float cameraRotationLimit = 85f;
   
    // Start is called before the first frame update
    void Start()
    {
        rb = GetComponent<Rigidbody>();
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        if(velocity != Vector3.zero) // move player
        {
            rb.MovePosition(rb.position + velocity*Time.fixedDeltaTime);
        }

        if(rotationPlayer != Vector3.zero) // rotate player horizontally
        {
            rb.MoveRotation(rb.rotation * Quaternion.Euler(rotationPlayer));
        }

        if(pCam != null) // rotate player vertically,(camera)
        {
            rotationCamera =  Mathf.Clamp(rotationCamera, -cameraRotationLimit, cameraRotationLimit); // clamp cameraRotation for not being reversed
            pCam.transform.localEulerAngles = new Vector3(rotationCamera, 0f, 0f);
        }

        if(jumpForce > 0) // if player push jump button, constantly add jump force
        {
            rb.AddForce(new Vector3(0, jumpForce,0), ForceMode.Impulse);
        }
    }
   
    public void Move(Vector3 _velocity)
    {//Get Playercontroller Velocity
        velocity = _velocity;
    }
    
    public void RotatePlayer(Vector3 _rotationPlayer)
    {
        rotationPlayer = _rotationPlayer;
    }

    public void RotateCamera(float _rotationCamera)
    {
        rotationCamera = _rotationCamera;
    }

    public void JumpPlayer(float _jumpForce)
    {
        jumpForce = _jumpForce;
    }
}
