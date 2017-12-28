---
title: LugBench Architecture
menu:
- title: Documentation
  href: /doc
  class: documentation button button-green align-right
---

# Introduction

The purpose of this section is to give you an overview of the internal workings of the Lugbench application.

# Application

The `Application` class is the base for any and all Lugdunum application.
This class is in charge of handling the startup and the shutdown of your application.

You will find all the startup code in `Application::init`.
In here we initialize the Vulkan renderer, as well as the camera. We also load the fonts and images at this point. The final step involves creating the first State of our application.

Each frame the function `Application::onFrame` is called.

When an event is received the function `Application::onEvent` is called.

In our case, both of these functions call an equivalent function of the currently active `State`.

# States

The `AState` class is the interface for the various "States" that we have in the Lugbench application. In this case, a State represents a section of the Application. LugBench is separated in 3 States, ModelsState, InfoState, and ContactState. We chose to organize things this way so that we could separate each functionality in its own class and we can easily switch between them at runtime.

Each state has an `onFrame` and `onEvent` that will be called by the Application. They also have an `onPush` and `onPop` that are the equivalents to a constructor and destructor.

## ModelsState

`ModelsState::loadModel` is called in `ModelsState::onPush` and is in charge of loading the models. The list of models that will be loaded is defined as a static array at the top of ModelsState.cpp 

`ModelsState::loadModelSkyBox` is called in `ModelsState::loadModel` and is in charge of loading the skybox. The list of skyboxes that will be loaded is defined as an unordered map at the top of ModelsState.cpp

The `ModelViewer` Class is what controls the camera's movement.


## InfoState

All of the information displayed in this State is already retrieved by Lugdunum, so here we simply retrieve the data from a `PhysicalDeviceInfo` which is the only member variable of this State.
The data is then displayed by the use of Dear Imgui that we support as our GUI library.

## ContactState

This State is separated into 3 sub-states; this was not for any particular reason we just decided to organize things that way.
