using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyDetector : MonoBehaviour
{
    public GameObject Spy;
    public Pathfinding.AIDestinationSetter DestSetterScript;
    public Pathfinding.Patrol PatrolScript;
    public Pathfinding.AILerp AILerpScript;

    public GameObject LastSeen;

    public Transform LastLocation;
    public bool ArrivalCheck;

    public bool HuntingSpy;

    public int Food;
    public float FoodTimer;
    public GameObject KitchenPoint;
    public bool GoingToKitchen;

    public float Dist;

    // Start is called before the first frame update
    void Start()
    {
        ArrivalCheck = false;
        Spy = GameObject.Find("Spy");
        DestSetterScript = gameObject.GetComponentInParent<Pathfinding.AIDestinationSetter>();
        DestSetterScript.target = this.transform;
        PatrolScript = gameObject.GetComponentInParent<Pathfinding.Patrol>();
        HuntingSpy = false;
        Food = Random.Range(30,50);
        FoodTimer = Time.time + 5;
        GoingToKitchen = false;
        PatrolScript.delay = Random.Range(1.0f, 5.0f);
    }

    public void OnTriggerStay2D(Collider2D collision)
    {
        //If Spy has entered cone and is not currently hiding
        if (collision.gameObject.CompareTag("Spy") && Spy.GetComponent<SpyScript>().IsHiding == false)
        {
            SpyCheck();
        }

        //If Collision is a door
        else if (collision.gameObject.CompareTag("Door"))
        {
            InspectDoor(collision.gameObject);
        }
    }

    public void OnTriggerExit2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Spy")
        {
            SpyExitCheck();
        }
    }

    public void Update()
    {
        //Decrease food by 5
        if (Time.time > FoodTimer)
        {
            Food -= 5;
            FoodTimer = Time.time + 5;
        }

        Dist = Vector2.Distance(this.transform.parent.position, DestSetterScript.target.transform.position);
        //If chasing the spy but no longer should be
        if ((ArrivalCheck && Dist <= 1) || (HuntingSpy == true && Spy.GetComponent<SpyScript>().IsHiding == true))
        {
            ReturnToPatrol();
        }

        //If going to Kitchen and arrived
        if(GoingToKitchen == true && Dist <= 2)
        {
            GoingToKitchen = false;
            Food = Random.Range(30, 50);
        }


        //Else if not currently hunting the spy and Guards food is less than 20
        else if(Food <= 20 && HuntingSpy == false)
        {
            PatrolScript.enabled = false;
            DestSetterScript.enabled = true;
            DestSetterScript.target = KitchenPoint.transform;
            GoingToKitchen = true;
        }

    }

    //AI returns to patrolling
    public void ReturnToPatrol()
    {
        //IF hunting door
        if (DestSetterScript.target.CompareTag("Door"))
        {
            foreach(GameObject Door in DestSetterScript.target.GetComponent<DoorRotation>().Doors)
            {
                DestSetterScript.target.GetComponent<DoorRotation>().DoOpenDoor(Door);
                DestSetterScript.target.GetComponent<DoorRotation>().OpenDoor = false;
            }
        }

        //Return to Patrol
        AILerpScript.speed = 4;
        LastSeen.transform.position = new Vector2(30, 30);
        PatrolScript.enabled = true;
        DestSetterScript.enabled = false;
        ArrivalCheck = false;
        HuntingSpy = false;
        Spy.GetComponent<SpyScript>().Spotted = false;
    }

    //When Spy leaves Cone of view
    public void SpyExitCheck()
    {
        //Checking if the spy is behind a wall
        //Raycast to Spy
        RaycastHit2D hit = Physics2D.Raycast(this.transform.position, Spy.transform.position - this.transform.position, Mathf.Infinity, ~(LayerMask.GetMask("Cone")));
        Debug.DrawRay(this.transform.position, Spy.transform.position - this.transform.position, Color.red);
        //If hits spy without hitting wall
        if (hit.collider != null && hit.transform.gameObject.tag == "Spy")
        {
            //Go to last known position
            Debug.Log("Spy Left, Checking Last Known Location");
            LastSeen.transform.position = new Vector2(Spy.transform.position.x, Spy.transform.position.y);
            LastLocation = LastSeen.transform;
            DestSetterScript.target = LastLocation;
            Debug.Log(LastLocation);
            //Start checking for arrival at position
            ArrivalCheck = true;
        }
    }

    public void SpyCheck()
    {
        //Raycast to Spy
        RaycastHit2D hit = Physics2D.Raycast(this.transform.position, Spy.transform.position - this.transform.position, Mathf.Infinity, ~(LayerMask.GetMask("Cone")));
        Debug.DrawRay(this.transform.position, Spy.transform.position - this.transform.position, Color.red);
        if (hit.collider != null && hit.transform.gameObject.tag == "Spy")
        {
            Debug.Log("Spy Found");
            AILerpScript.speed = 5f;
            DestSetterScript.enabled = true;
            DestSetterScript.target = Spy.transform;
            PatrolScript.enabled = false;
            ArrivalCheck = false;
            Spy.GetComponent<Pathfinding.AILerp>().speed = 4;
            Spy.GetComponent<SpyScript>().Spotted = true;
            HuntingSpy = true;
        }
    }

    public void InspectDoor(GameObject Door)
    {
        //Check for line of sight
        RaycastHit2D hit = Physics2D.Raycast(this.transform.position, Door.transform.position - this.transform.position, Mathf.Infinity, ~(LayerMask.GetMask("Cone")));
        Debug.DrawRay(this.transform.position, Door.transform.position - this.transform.position, Color.green);
        if (hit.transform.gameObject.CompareTag("Door"))
        {
            //Check if door is open
            if (hit.transform.gameObject.GetComponent<DoorRotation>().OpenDoor == true)
            {
                //If Open, investigate
                DestSetterScript.target = hit.transform.gameObject.transform;
                DestSetterScript.enabled = true;
                PatrolScript.enabled = false;
                ArrivalCheck = true;
            }
        }
    }
}
