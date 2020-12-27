using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ObjectiveSpawner : MonoBehaviour
{
    public GameObject Objective;

    public List<GameObject> SpawnPoints;


    // Start is called before the first frame update
    void Start()
    {
        //Get Random Point
        int I = Random.Range(0, SpawnPoints.Count);
        Instantiate(Objective, new Vector3(SpawnPoints[I].transform.position.x, SpawnPoints[I].transform.position.y, 0), SpawnPoints[I].transform.rotation);
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
