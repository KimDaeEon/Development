#pragma strict

	private var ftime : float = 0.0;
	// Update is called once per frame
	function Update () {
		ftime = Time.deltaTime;
		if (ftime > 1) {
			Application.LoadLevel ("1st_Level");
		}
		if(Input.GetButtonDown("Cancel")){
			Application.Quit();
		}
	}