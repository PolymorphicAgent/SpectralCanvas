# Spectral Canvas

_*GOALS:*_
- To visualize sound in a very customizable manner.
    - i.e. customizable frequency range, colors, etc.
- To allow real-time editing of sound within the spectrograph with a variety of brushes/tools.

# Setting up the environment

**ONLY TESTED ON WINDOWS 64 bit - Any other OS's are not guaranteed to work.**

1) Installing / Setting up QT

    - Please note that this will take up a ton of storage space depending on the components you install. (the source code is massive)

        a) Download the QT installer from [this site](https://www.qt.io/download-qt-installer-oss).
        
        b) You may have to create a free account with QT.
        
        c) I reccomend keeping the default installation directory (at C:/Qt)
        
        d) Select "Qt x.x for desktop development"
        
        e) Finish the installation process. It will take a while!

2) Cloning this project via git

    - Once QT installed, you are ready to clone this project and set up the enivronment!

        a) Open command prompt to your Documents folder, making sure it's NOT your OneDrive Documents (onedrive will attempt to upload everything...) or wherever you keep your projects

        - Note: you must have [git for windows](https://git-scm.com/downloads/win) installed for this next step if you wish to be able to contribute to the project. 
        - You may skip this and directly download and unzip the source code, but you will be unable to contribute to the project.
        
        b) Run `git clone https://github.com/PolymorphicAgent/Spectrograph.git` after git has been installed
        - You should see a new folder named `Spectrograph' appear in your Documents folder (or wherever you decided to put it) 
        
3) Configuring the environment in QT creator
    - In file explorer, you should be able to right-click open the project file `Spectrograph.pro` with QT creator, the IDE that integrates best with the QT Framework.
    
        a) Once QT Creator loads, you should be on the "configure project screen". I suggest the use of `Desktop Qt 6.x.x MingGW 64bit` as your default kit.

        b) Hit `Configure Project" in the bottom corner.
        
4) Test Build!
    - Once you've got this environment set up, make sure "Debug" is selected as the build configuration in the bottom left corner.
    
        a) Hit the green play button to run the build process!
        
        b) If it compiles correctly, **you shouldn't get any errors**. Errors signify a misconfigured environment.
        
5) Contributing to the project
    - Before making changes, make sure you have the latest changes.
    
        a) `git fetch`
    
        b) `git pull`
        
    - You're good to make changes!
    
    - **If you want to become a contributor, please contact me.**
    - To submit changes to the project as a non-contributor, a Pull Request (PR) is needed.
    
        a) Learn about Pull Requests and how to make them [here](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-a-pull-request).

