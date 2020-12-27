using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class SelectorNode
{
    public LeafNode LeafToCheck;
    public SelectorNode NextSelector;
}

public class LeafNode
{
    public delegate bool MethodToCall();
    public MethodToCall LeafMethod;
}

public class BehaviourTreeScript : MonoBehaviour
{
    public AgentBehaviour GuardBehaviour;
    private SelectorNode CurrentNode;

    //Create Selector Nodes
    private SelectorNode ChaseSelector = new SelectorNode();
    private SelectorNode HuntingSelector = new SelectorNode();
    private SelectorNode DoorSelector = new SelectorNode();
    private SelectorNode HungerSelector = new SelectorNode();
    private SelectorNode PatrolSelector = new SelectorNode();

    //Create Leaf Nodes
    private LeafNode ChaseCheck = new LeafNode();
    private LeafNode HuntingCheck = new LeafNode();
    private LeafNode DoorCheck = new LeafNode();
    private LeafNode KitchenCheck = new LeafNode();
    private LeafNode Patrol = new LeafNode();

    private void Start()
    {
        //Get Behaviour Script
        GuardBehaviour = this.gameObject.GetComponent<AgentBehaviour>();

        //SelectorNodeSetup
        ChaseSelector.LeafToCheck = ChaseCheck;
        ChaseSelector.NextSelector = HuntingSelector;

        HuntingSelector.LeafToCheck = HuntingCheck;
        HuntingSelector.NextSelector = DoorSelector;

        DoorSelector.LeafToCheck = DoorCheck;
        DoorSelector.NextSelector = HungerSelector;

        HungerSelector.LeafToCheck = KitchenCheck;
        HungerSelector.NextSelector = PatrolSelector;

        //Patrol Selector will always return true
        PatrolSelector.LeafToCheck = Patrol;

        ChaseCheck.LeafMethod = GuardBehaviour.ChaseTheSpy;
        HuntingCheck.LeafMethod = GuardBehaviour.MoveToLastLocation;
        DoorCheck.LeafMethod = GuardBehaviour.InvestigateDoor;
        KitchenCheck.LeafMethod = GuardBehaviour.MovingToKitchen;
        Patrol.LeafMethod = GuardBehaviour.AgentPatrol;
    }

    private void Update()
    {
        CurrentNode = ChaseSelector;
        CheckNode(CurrentNode);
    }

    private void CheckNode(SelectorNode CurrentNodeToCheck)
    {
        //Perform the nodes LeafCheck
        bool NodeStatus = CurrentNodeToCheck.LeafToCheck.LeafMethod();
        if (!NodeStatus)
        {
            //The Behaviour criteria was not met in some way
            CurrentNode = CurrentNodeToCheck.NextSelector;
            CheckNode(CurrentNode);
        }
    }

}
