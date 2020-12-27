using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class SpyScript : MonoBehaviour
{
    public Pathfinding.AIDestinationSetter ThisDestSetter;
    public GameObject Objective;

    public bool Spotted;
    public bool IsHiding;

    public bool SpySpotted;

    public List<GameObject> HidingSpots;


    // Start is called before the first frame update
    void Start()
    {
        SpySpotted = false;
        ThisDestSetter = this.GetComponent<Pathfinding.AIDestinationSetter>();
        IsHiding = false;
    }

    // Update is called once per frame
    void Update()
    {
        this.gameObject.transform.root.position = new Vector3(this.transform.root.position.x, this.transform.root.position.y, 0);

        //Find Nearest Crate and Hide
        if (SpySpotted == true)
        {
            ThisDestSetter.target = Flee().transform;
        }

        //Find Objective
        if (Objective == null)
        {
            Objective = GameObject.FindGameObjectWithTag("Objective");
            ThisDestSetter.target = Objective.transform;
        }

        else if (IsHiding)
        {
            WaitForOpening();
        }
    }

    private void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.tag == "Guard" || collision.gameObject.tag == "Objective")
        {
            //Spy Has Been Caught or has reached its objective
            SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex);
        }
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        //Set To Be Hiding
        if (collision.gameObject.tag == "HidingCrate")
        {
            IsHiding = true;
        }
    }

    private void OnTriggerExit2D(Collider2D collision)
    {
        //Set To Not Be Hiding
        if (collision.gameObject.tag == "HidingCrate")
        {
            IsHiding = false;
        }
    }

    public GameObject Flee()
    {
        float ClosestSpot = int.MaxValue;
        GameObject ClosestObject = HidingSpots[0];
        //Find Nearest Hiding Spot
        foreach(GameObject Spot in HidingSpots)
        {
            //Check crate in relation to spy and guard

            if(Vector2.Distance(this.gameObject.transform.position, Spot.transform.position) < ClosestSpot)
            {
                ClosestSpot = Vector2.Distance(this.gameObject.transform.position, Spot.transform.position);
                ClosestObject = Spot;
            }
        }

        //Set to Target
        return ClosestObject;
    }

    public void WaitForOpening()
    {
        int GuardCount = 0;
        foreach (GameObject Guard in GameObject.FindGameObjectsWithTag("Guard"))
        {
            //If Guard is more than 50 away
            if(Vector2.Distance(this.gameObject.transform.position, Guard.transform.position) > 10)
            {
                GuardCount += 1;
            }
        }

        //If all guards are more than 10
        if(GuardCount == 3)
        {
            ThisDestSetter.target = Objective.transform;
        }
    }
}
