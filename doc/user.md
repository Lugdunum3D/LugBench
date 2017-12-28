---
title: How to use LugBench
menu:
- title: Documentation
  href: /doc
  class: documentation button button-green align-right
---

# What is Lugbench

Lugbench is a proof of concept developed to show what was possible with our 3D Engine Lugdunum. In its current form, you can use Lugbench to visualize a few 3D models as well as information regarding your rendering hardware.

# How to use the desktop application

When starting the application you will have the choice between 3 tabs listed at the top of the window.

![Navigation Bar](https://i.imgur.com/usTYWsL.png)

We will go over each of them in order and explain what their purpose is.

## Models

This will be the first thing you see when starting the application. This part is split into 2 important sections: The viewing window on the right and the selection list on the left.

![The model view](https://i.imgur.com/3H5dCsD.png)

The Selection list lets you chose between several models to view.

The Viewing window is where you can view and interact with the currently selected 3D model. Click and drag on pc or use your finger on touch devices to rotate the camera. You can also zoom with your scroll wheel on pc or by pinching with your fingers on touch devices.
On the bottom of the viewing window there are 3 buttons:
- The first, starting from the left, will activate/deactivate the fullscreen mode. In this mode the viewing window will take up all the screen, hiding the rest of the UI.
- The middle button will switch between several rendering modes showing you all the parts that go into creating the final result.
In order these modes are:
    - Full
    - Albedo Only
    - Normal Only
    - Metallic Only
    - Roughness Only
    - Ambien Occlusion only
    - Ambient Occlusion, Roughness, and Metallic
    - Emissive Only
- The third and final button shows and hides the skybox

## Info

This window will list information about either your dedicated or integrated graphic device.

![The info view displays the device's characteristics](https://i.imgur.com/AqBAd4B.png)

The device that is currently being used will be shown on the top left.
The additional information will be displayed on the right.
These are things like the graphic features, extensions and queue families supported by your device. These are things that might be usefull to know if you wish to develop Vulkan applications with your device.

## Contact

Here you will find various legal and contact information.

![The contact view](https://i.imgur.com/C5KyLMc.png)

In the Authors section, you can find the names and contacts of all the members of the development team.
In the Licence section, you can find the licenses for all the libraries we use.
In the Contact section, you can find the team's contact details as well as our Twitter and Github page 