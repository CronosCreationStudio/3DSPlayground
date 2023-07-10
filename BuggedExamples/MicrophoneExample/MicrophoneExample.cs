using UnityEngine;
using System.Collections;

public class MicrophoneExample : MonoBehaviour
{
	void Start()
	{
		foreach (string device in Microphone.devices)
		{
			Debug.Log("Found Microphone: '" + device + "'");
		}

		deviceName = Microphone.devices[0];
		audioSource = GetComponent<AudioSource>();
	}

	void OnGUI()
	{
		int xpos = 160 - (width / 2);
		int ypos = 20;

		if (ButtonPressed(xpos, ref ypos, "Power On: ", ref isPowerEnabled))
		{
			if (isPowerEnabled)
			{
				UnityEngine.N3DS.Microphone.Enable();
			}
			else
			{
				UnityEngine.N3DS.Microphone.Disable();
			}
		}

		if (isPowerEnabled)
		{
			if (Microphone.IsRecording(deviceName) == false)
			{
				if (ButtonPressed(xpos, ref ypos, "Start Recording"))
				{
					bool loop = false;
					int lengthSeconds = 3;
					int frequency = 32728;
					audioSource.clip = Microphone.Start(deviceName, loop, lengthSeconds, frequency);
					hasRecorded = true;
				}
			}
			else
			{
				if (ButtonPressed(xpos, ref ypos, "Stop Recording"))
				{
					Microphone.End(deviceName);
				}
			}
		}

		if (hasRecorded)
		{
			if (ButtonPressed(xpos, ref ypos, "Play"))
			{
				audioSource.Play();
			}
		}
	}

	private bool ButtonPressed(int xpos, ref int ypos, string label)
	{
		bool result = GUI.Button(new Rect(xpos, ypos, width, height), label);
		ypos += height + spacingV;
		return result;
	}

	private bool ButtonPressed(int xpos, ref int ypos, string label, ref bool flag)
	{
		bool result = false;
		if (GUI.Button(new Rect(xpos, ypos, width, height), label + flag))
		{
			flag = !flag;
			result = true;
		}
		ypos += height + spacingV;
		return result;
	}

	private string deviceName = null;
	private AudioSource audioSource;

	private bool isPowerEnabled;
	private bool hasRecorded;

	private const int width = 120;
	private const int height = 32;
	private const int spacingV = 10;
}
