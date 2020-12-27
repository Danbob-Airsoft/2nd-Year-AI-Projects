using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DoorRotation : MonoBehaviour
{
    public bool OpenDoor;
    public List<GameObject> Doors;
    public GameObject[] Guards;

    private void Start()
    {
        OpenDoor = false;
        Doors.Add(this.transform.GetChild(0).gameObject);
        Doors.Add(this.transform.GetChild(1).gameObject);
        Guards = GameObject.FindGameObjectsWithTag("Guard");
    }

    public void DoOpenDoor(GameObject Door)
    {
        if (!OpenDoor)
        {
            OpenDoor = true;
            if (Door.gameObject.transform.CompareTag("Clockwise"))
            {
                Door.transform.rotation *= Quaternion.Euler(0, 0, -90);
            }

            else if (Door.transform.CompareTag("AntiClockwise"))
            {
                Door.transform.rotation *= Quaternion.Euler(0, 0, 90);
            }
        }

        else if (OpenDoor)
        {
            OpenDoor = false;
            if (Door.transform.CompareTag("Clockwise"))
            {
                Door.transform.rotation *= Quaternion.Euler(0, 0, -90);
            }

            else if (Door.gameObject.transform.CompareTag("AntiClockwise"))
            {
                Door.transform.rotation *= Quaternion.Euler(0, 0, 90);
            }
        }
    }

    public void OnTriggerEnter2D(Collider2D collision)
    {
        if(collision.gameObject.CompareTag("Spy"))
        {
            foreach (GameObject Door in Doors)
            {
                DoOpenDoor(Door);
            }

            //Check for Guards in range to hear doors open
            foreach (GameObject Guard in Guards)
            {
                Vector3 VectToDoor = this.gameObject.transform.position - Guard.transform.position;
                float Distance = VectToDoor.magnitude;

                //If Distance is less than 10
                if(Distance <= 10)
                {
                    //Set Door to Guard Target
                    Guard.GetComponent<Pathfinding.AIDestinationSetter>().target = this.gameObject.transform;
                    Guard.GetComponent<Pathfinding.AIDestinationSetter>().enabled = true;
                    Guard.GetComponent<Pathfinding.Patrol>().enabled = false;
                    //Set Guard to check for arrival at door
                    Guard.GetComponentInChildren<AgentBehaviour>().DoorOpenedNearby = true;
                    Guard.GetComponentInChildren<AgentBehaviour>().LastDoorOpened = this.gameObject;
                }
            }
        }

        else if (collision.gameObject.CompareTag("Guard"))
        {
            foreach (GameObject Door in Doors)
            {
                DoOpenDoor(Door);
            }
        }
    }

    public void OnTriggerExit2D(Collider2D collision)
    {
        if (collision.gameObject.CompareTag("Guard"))
        {
            foreach (GameObject Door in Doors)
            {
                DoOpenDoor(Door);
            }
        }
    }
}
