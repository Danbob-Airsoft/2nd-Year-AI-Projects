using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AgentBehaviour : MonoBehaviour
{
    public GameObject Spy;
    public bool SpyInCone = false;
    public bool GoingToLastSeenLocation = false;
    public bool DoorOpenedNearby = false;
    public bool InvestigatingDoor = false;

    public GameObject LastSeen;
    public Transform LastLocation;

    public GameObject LastDoorOpened;

    //Food Meter stuff
    public int GuardFood;
    public float FoodDecreaseTime;
    public bool OnWayToKitchen;
    public GameObject KitchenPoint;

    //--------------------------------------------- A* Scripts --------------------------
    public Pathfinding.AIDestinationSetter DestSetterScript;
    public Pathfinding.Patrol PatrolScript;
    public Pathfinding.AILerp AILerpScript;

    // Start is called before the first frame update
    void Start()
    {
        GuardFood = Random.Range(25, 50);
        FoodDecreaseTime = Time.time + Random.Range(5, 15);
        Spy = GameObject.FindGameObjectWithTag("Spy");
        DestSetterScript = this.gameObject.GetComponentInParent<Pathfinding.AIDestinationSetter>();
        DestSetterScript.target = this.transform;
        PatrolScript = this.gameObject.GetComponentInParent<Pathfinding.Patrol>();
        PatrolScript.delay = Random.Range(1.0f, 5.0f);
        AILerpScript = this.gameObject.GetComponentInParent<Pathfinding.AILerp>();
        OnWayToKitchen = false;
    }

    // Update is called once per frame
    void Update()
    {
        //Reduce guards hunger
        if (Time.time > FoodDecreaseTime)
        {
            FoodDecreaseTime = Time.time + Random.Range(5, 15);
            GuardFood -= 3;
        }

        this.gameObject.transform.root.position = new Vector3(this.transform.root.position.x, this.transform.root.position.y, 0);
    }

    private void OnTriggerStay2D(Collider2D collision)
    {
        if (collision.gameObject.CompareTag("Spy") && !Spy.GetComponent<SpyScript>().IsHiding)
        {
            SpyInCone = true;
        }
    }

    private void OnTriggerExit2D(Collider2D collision)
    {
        if (collision.gameObject == Spy)
        {
            SpyInCone = false;
        }
    }

    public bool ChaseTheSpy()
    {
        //If the spy is in cone of sight and not hiding
        if (SpyInCone == true && !Spy.GetComponent<SpyScript>().IsHiding)
        {
            Debug.Log("Spy In Cone");
            //Check for Direct line (Not through walls)
            RaycastHit2D hit = Physics2D.Raycast(this.transform.position, Spy.transform.position - this.transform.position, Mathf.Infinity, ~(LayerMask.GetMask("Cone")));
            Debug.DrawRay(this.transform.position, Spy.transform.position - this.transform.position, Color.red);
            if (hit.collider != null && hit.transform.gameObject.tag == "Spy")
            {
                Debug.Log("Visual Confirmed");
                Debug.Log("Chase The Spy");
                Spy.GetComponent<SpyScript>().SpySpotted = true;
                // if previously investigating door or moving to last location, reset these
                if (GoingToLastSeenLocation)
                {
                    GoingToLastSeenLocation = false;
                }
                if (InvestigatingDoor)
                {
                    InvestigatingDoor = false;
                }
                AILerpScript.speed = 5f;
                DestSetterScript.enabled = true;
                DestSetterScript.target = Spy.transform;
                PatrolScript.enabled = false;
                Spy.transform.root.GetComponent<Pathfinding.AILerp>().speed = 4;
                Spy.transform.root.GetComponent<SpyScript>().ThisDestSetter.target = Spy.transform.root.GetComponent<SpyScript>().Flee().transform;
                return true;
            }
            return false;
        }
        return false;
    }

    public bool MoveToLastLocation()
    {
        if (Spy.GetComponent<SpyScript>().SpySpotted || GoingToLastSeenLocation)
        {
            if (Spy.GetComponent<SpyScript>().SpySpotted)
            {
                LastSeen.transform.position = new Vector2(Spy.transform.position.x, Spy.transform.position.y);
                LastLocation = LastSeen.transform;
                DestSetterScript.target = LastLocation;
                Spy.GetComponent<SpyScript>().SpySpotted = false;
                GoingToLastSeenLocation = true;
            }
            Debug.Log("Moving to last known position");
            //Check if arrived
            float Dist = Vector2.Distance(this.transform.parent.position, DestSetterScript.target.transform.position);
            if ((Dist <= 1) || (GoingToLastSeenLocation == true && Spy.GetComponent<SpyScript>().IsHiding == true))
            {
                GoingToLastSeenLocation = false;
            }
            return true;
        }
        return false;
    }

    public bool InvestigateDoor()
    {
        if (DoorOpenedNearby || InvestigatingDoor)
        {
            if (DoorOpenedNearby)
            {
                DoorOpenedNearby = false;
                InvestigatingDoor = true;
            }
            //Check for line of sight
            RaycastHit2D hit = Physics2D.Raycast(this.transform.position, LastDoorOpened.transform.position - this.transform.position, Mathf.Infinity, ~(LayerMask.GetMask("Cone")));
            Debug.DrawRay(this.transform.position, LastDoorOpened.transform.position - this.transform.position, Color.green);
            if (hit.transform.gameObject.CompareTag("Door"))
            {
                //Check if door is open
                if (hit.transform.gameObject.GetComponent<DoorRotation>().OpenDoor == true)
                {
                    //If Open, investigate
                    DestSetterScript.target = hit.transform.gameObject.transform;
                    DestSetterScript.enabled = true;
                    PatrolScript.enabled = false;
                }
            }

            //Check arrival
            float Dist = Vector2.Distance(this.transform.parent.position, DestSetterScript.target.transform.position);
            if ((Dist <= 1) && InvestigatingDoor)
            {
                //Close door
                foreach (GameObject Door in DestSetterScript.target.GetComponent<DoorRotation>().Doors)
                {
                    DestSetterScript.target.GetComponent<DoorRotation>().DoOpenDoor(Door);
                    DestSetterScript.target.GetComponent<DoorRotation>().OpenDoor = false;
                }
                InvestigatingDoor = false;
            }
            return true;
        }
        return false;
    }

    public bool MovingToKitchen()
    {
        if (GuardFood <= 10 || OnWayToKitchen)
        {
            if (!OnWayToKitchen)
            {
                PatrolScript.enabled = false;
                DestSetterScript.enabled = true;
                DestSetterScript.target = KitchenPoint.transform;
                OnWayToKitchen = true;
            }

            //Check if arrived at Kitchen
            float Dist = Vector2.Distance(this.transform.parent.position, DestSetterScript.target.transform.position);
            if ((Dist <= 1) && InvestigatingDoor)
            {
                OnWayToKitchen = false;
                //If yes Replenish food
                GuardFood = Random.Range(25, 30);
            }
            return true;
        }
        return false;
    }

    public bool AgentPatrol()
    {
        //Return to Patrol
        AILerpScript.speed = 4;
        //LastSeen.transform.position = new Vector2(30, 30);
        PatrolScript.enabled = true;
        DestSetterScript.enabled = false;
        return true;
    }
}
