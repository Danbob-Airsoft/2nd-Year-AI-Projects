using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class MainMenuScript : MonoBehaviour
{
    public CanvasGroup MainMenuCanvas;
    public CanvasGroup SettingsCanvas;
    public CanvasGroup ControlsCanvas;
    public CanvasGroup StoryCanvas;

    public void PlayGame()
    {
        SceneManager.LoadScene(1);
    }

    public void OpenSettings()
    {
        MainMenuCanvas.alpha = 0;
        MainMenuCanvas.interactable = false;
        MainMenuCanvas.blocksRaycasts = false;
        SettingsCanvas.alpha = 1;
        SettingsCanvas.interactable = true;
        SettingsCanvas.blocksRaycasts = true;
    }

    public void OpenControls()
    {
        MainMenuCanvas.alpha = 0;
        MainMenuCanvas.interactable = false;
        MainMenuCanvas.blocksRaycasts = false;
        ControlsCanvas.alpha = 1;
        ControlsCanvas.interactable = true;
        ControlsCanvas.blocksRaycasts = true;
    }

    public void ReturnToMenu()
    {
        MainMenuCanvas.alpha = 1;
        MainMenuCanvas.interactable = true;
        MainMenuCanvas.blocksRaycasts = true;
        SettingsCanvas.alpha = 0;
        SettingsCanvas.interactable = false;
        SettingsCanvas.blocksRaycasts = false;
        ControlsCanvas.alpha = 0;
        ControlsCanvas.interactable = false;
        ControlsCanvas.blocksRaycasts = false;
    }

    public void OpenStory()
    {
        MainMenuCanvas.alpha = 0;
        MainMenuCanvas.interactable = false;
        MainMenuCanvas.blocksRaycasts = false;
        StoryCanvas.alpha = 1;
        StoryCanvas.interactable = true;
        StoryCanvas.blocksRaycasts = true;
    }

    public void Close()
    {
        Application.Quit();
    }
}
