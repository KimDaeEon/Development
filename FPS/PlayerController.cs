
using UnityEngine;


[RequireComponent(typeof(PlayerMotor))]
[RequireComponent(typeof(ConfigurableJoint))]
[RequireComponent(typeof(Animator))]
public class PlayerController : MonoBehaviour
{
    //Related with thruster and fuel
    [SerializeField] private float thrusterFuelBurnSpeed = 0.2f;
    [SerializeField] private float thrusterFuelRegenSpeed = 1f;
    [SerializeField] private float floatingOffSet = 0.3f; // Amount of floating on some object
    [SerializeField] private LayerMask collidableEnvironmentMask; // Player only floats on environment.   

    private float thrusterFuelAmount = 1f;
    [HideInInspector]public bool canFly = true;
   

    
    public float GetThrusterFuelAmount()
    {
        return thrusterFuelAmount;
    }


    //Related with thruster and fuel

    [Header("Spring Settings: ")]
    [SerializeField] private float yDrivePositionSpring = 20;
    [SerializeField] private float yDriveMaximumForce = 1000;

   
    [Header("PlayerMotor Settings: ")]
    [SerializeField] private float speed = 5.0f;
    [SerializeField] private float rotationSpeed = 15.0f;
    [SerializeField] private float cameraRotationSpeed = 15.0f;
    [SerializeField] private float jumpForce = 10.0f;
    
    public float cameraRotation = 0.0f;

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
        if (PauseMenu.IsPauseOn)
        {
            if(Cursor.lockState != CursorLockMode.None)
            {
                Cursor.lockState = CursorLockMode.None;
                Cursor.visible = true;
            }
            pMotor.Move(Vector3.zero); // while pause cannot move

            pMotor.JumpPlayer(0); // while pause cannot jump

            //while pause cannot rotate camera
            pMotor.RotatePlayer(Vector3.zero);

            //while pause fuel regens
            thrusterFuelAmount += thrusterFuelRegenSpeed * Time.deltaTime;
            thrusterFuelAmount = Mathf.Clamp(thrusterFuelAmount, 0f, 1f);

            if (thrusterFuelAmount >= 1f)
            {
                canFly = true;
            }
            return;
        }

        if(Cursor.lockState != CursorLockMode.Locked)
        {
            Cursor.lockState = CursorLockMode.Locked;
            Cursor.visible = false;
        }

        RaycastHit _hit;
        if(Physics.Raycast(transform.position, Vector3.down, out _hit, 100f,collidableEnvironmentMask))
        {
            pJoint.targetPosition =  new Vector3(0f, -_hit.point.y - floatingOffSet, 0f);
            // 0.5 need to be plus for player being floating
        }
        else
        {
            pJoint.targetPosition = Vector3.zero;
        }
        //Get direction information from keyboard
        float xMove = Input.GetAxis("Horizontal");
        float zMove = Input.GetAxis("Vertical");

        //Get yRotation input
        float yRotate = Input.GetAxisRaw("Mouse X");
        //Get xRotation input
        float xRotate = Input.GetAxisRaw("Mouse Y");

        float _thrusterForce = 0f;

        if (Input.GetButton("Jump") && canFly)
        {
            //Get ThrustForce Input
            float yJump = Input.GetAxis("Jump");
            thrusterFuelAmount -= thrusterFuelBurnSpeed * Time.deltaTime;
            _thrusterForce = yJump * jumpForce;
            if(thrusterFuelAmount <= 0)
            {
                canFly = false;
            }
        }
        else
        {
            thrusterFuelAmount += thrusterFuelRegenSpeed * Time.deltaTime;
            if(thrusterFuelAmount >= 1f)
            {
                canFly = true;
            }
        }

        thrusterFuelAmount = Mathf.Clamp(thrusterFuelAmount, 0f, 1f);

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
        pMotor.JumpPlayer(_thrusterForce);

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
