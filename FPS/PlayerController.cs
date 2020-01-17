
using UnityEngine;
[RequireComponent(typeof(PlayerMotor))]
[RequireComponent(typeof(ConfigurableJoint))]
[RequireComponent(typeof(Animator))]
public class PlayerController : MonoBehaviour
{


    [Header("Spring Settings: ")]
    [SerializeField] private float yDrivePositionSpring = 20;
    [SerializeField] private float yDriveMaximumForce = 1000;

   
    [Header("PlayerMotor Settings: ")]
    [SerializeField] private float speed = 5.0f;
    [SerializeField] private float rotationSpeed = 15.0f;
    [SerializeField] private float cameraRotationSpeed = 15.0f;
    [SerializeField] private float jumpForce = 10.0f;

    float cameraRotation = 0.0f;

    // Components Required
    private PlayerMotor pMotor;
    private ConfigurableJoint pJoint;
    private Animator pAnimator;
    // Start is called before the first frame update
    void Start()
    {
        pMotor = GetComponent<PlayerMotor>();
        pJoint = GetComponent<ConfigurableJoint>();
        pAnimator = GetComponent<Animator>();
        
        SetConfigurableJoint();
    }

    // Update is called once per frame
    void Update()
    {
        //Get direction information from keyboard
        float xMove = Input.GetAxis("Horizontal");
        float zMove = Input.GetAxis("Vertical");

        //Get yRotation input
        float yRotate = Input.GetAxisRaw("Mouse X");
        //Get xRotation input
        float xRotate = Input.GetAxisRaw("Mouse Y");

        //Get ThrustForce Input
        float yJump = Input.GetAxis("Jump");
        //Caculate direction vector for move
        Vector3 moveHorizontal = transform.right * xMove;
        Vector3 moveVertical = transform.forward * zMove;

        //Normalized velocity means direction and multiplying it by speed means real velocity
        Vector3 velocity = (moveHorizontal + moveVertical).normalized * speed;

        //Apply calculated velocity to PlayerMotor's velocity
        pMotor.Move(velocity);

        //Set rotation factor, before applying this to PlayerMotor        
        Vector3 yRotationVec = new Vector3(0, yRotate, 0) * rotationSpeed;
        cameraRotation += xRotate * cameraRotationSpeed;
        pMotor.RotatePlayer(yRotationVec);
        pMotor.RotateCamera(-cameraRotation); // negate xRotationVec makes mouse move and rotation synchronization

        //Set ThrustForce to PlayerMotor
        pMotor.JumpPlayer(yJump * jumpForce);


        // Animate movement
        pAnimator.SetFloat("ForwardVelocity", zMove);
    }

    private void SetConfigurableJoint()
    {
        pJoint.yDrive = new JointDrive {
            positionSpring = yDrivePositionSpring,
            maximumForce = yDriveMaximumForce
        };
    }

}
