---
icon: lucide/badge-info
---
# Getting Started

This short introduction will briefly guide you through the project setup and the user interface of NumbatUI. The bullet point lists will provide you with the next steps to take:

## Starting Up NumbatUI

![Start Windows](documentation/start_window.png){ width="400", align="right" }

Once you've installed NumbatUI successfully you are ready to run the application. For assistance with installation, visit the [installation](!installation) section.

!!! example "Tasks"
    * Launch NumbatUI.

After launching NumbatUI you will see the [Start Window](!interface#start-window). From here you can create your own project or choose a pre-indexed one.

!!! example "Tasks"
    * Click **New Project** to create a new project.
    * or select one from the **Recent Projects** _(ex: TicTacToe)_ and continue with the [UI Intro](!interface#ui-intro)



## Creating a New Project

When creating a new NumbatUI project you will use the [Project Setup Wizard](!interface#project-setup-wizard). This wizard splits the setup process into several subsequent steps. Depending on your project's structure and the used build system, there are different types for project setup. Choosing the correct setup method is important and can make the setup process a lot easier.

!!! example "Tasks"
    * Give your project a **Name** and select a **Location** for your NumbatUI project to live.
    * Click **Add Source Group** to add source files to the project.

<img src="documentation/project_setup_wizard_start.png" height="500" alt="Project Setup Wizard Start">

### Add Source Group

NumbatUI projects consist of multiple *Source Groups*. Each Source Group uses a certain language, a set of files, and all configurations to index these files. There are different types of Source Groups for each supported programming language. In addition, creating a single Source Group is sufficient for most projects.

Scroll past the image for detailed instructions on setting this up.

!!! example "Tasks"
    * Select your chosen **Source Group** setup type and come back here as soon as the project is created.

<img src="documentation/project_setup_wizard_source_group_type.png" width="500" alt="Project Setup Wizard Source Group Type">

=== "Source Group Setup for C/C++"

    The Source Group setup types for C & C++ are the same.

    **Are you using CMake, Make or Qt Creator?**

    If you are using [CMake](https://cmake.org/) or [Make](https://www.gnu.org/software/make/) as build environment you can export a [clang JSON Compilation Database](https://clang.llvm.org/docs/JSONCompilationDatabase.html) as `compile_commands.json` file. A Compilation Database holds all information necessary for building the project, such as source files, include paths and compiler flags. Having a Compilation Database makes project setup within NumbatUI a lot easier. We recommend using this approach if possible.

    Exporting a Compilation Database:

    * From **CMake** by defining the `CMAKE_EXPORT_COMPILE_COMMANDS` flag. (not supported on for Visual Studio CMake generators)
    * For **Make** projects use [Bear](https://github.com/rizsotto/Bear). Bear generates a `compile_commands.json` file during a simulated build process. Bear has been tested on FreeBSD, GNU/Linux and OS X.
    * From **Qt Creator since version 4.8** by selecting the "Generate Compilation Database" from the "Build" menu.

    If you managed to export a `compile_commands.json` file, then please continue at [Create a Source Group from Compilation Database](!interface#create-a-project-from-compilation-database) and come back after you finished creating the project.

    <img src="documentation/project_setup_wizard_start_cdb.png" width="500" alt="Project Setup Wizard Start CDB">



    **Create Empty**

    If neither of the above options apply to your project, please continue at [create an empty C/C++ Source Group](!interface#create-an-empty-cxx-project) and return here once the project is created.

    <img src="documentation/project_setup_wizard_source_group_type.png" width="500" alt="Project Setup Wizard Source Group Type">

=== "Source Group Setup for Python"

    **Create empty**

    If you want to browse your Python source code with NumbatUI, please continue at [Create an Empty Python Source Group](!interface#create-an-empty-python-project) and come back here as soon as the project is created.

    <img src="documentation/project_setup_wizard_start_python_empty.png" width="500" alt="Project Setup Wizard Start Python Empty">

### Source Indexing

After the project is created, NumbatUI will ask you whether you want to start indexing. Click **Start** and wait for the indexing to complete. This may take a bit of time. The [Indexing Dialog](!interface#indexing-dialogs) and the [Status Bar](!interface#status-bar) will give you information about the progress. Otherwise the UI will be empty. NumbatUI indexes all named symbols and their relationships throughout the provided source files.

!!! example "Tasks"
    * Click **Start** when asked whether to start indexing.
    * Wait until the indexing of your source files has finished.
    * or Click **Stop** or press ESC to stop indexing (NumbatUI will provide all information gathered so far and the indexing can be continued later by [refreshing](!interface#refresh)).

!["Indexing"](documentation/indexing.png "Indexing")

After indexing is completed, NumbatUI will show an overview of all indexed symbols in the [graph view](!interface#graph-view) and some statistics in the [code view](!interface#code-view).

### Troubleshooting Errors

If the indexing yields errors, the status view will be shown with a list of errors. You can click on the errors label on the right side of the status bar or on one of the errors in the table to see their location.

!!! example "Tasks"
    * Fix your errors and [refresh](!interface#refresh) to re-index the files with errors (Open Issue: As long as there was no change in the specific file, NumbatUI won't reindex it, use the **Force Refresh** option from the [Edit Menu](!interface#edit)).
    * or ignore them and continue with an incomplete index.

!["Error View"](documentation/error_view.png "Error View")

## User Interface

As mentioned earlier, NumbatUI's user interface is split into three main views. Their arrangement can be adjusted as preferred and can also be detached from the main window and split into different screens (see [Window Widgets](!interface#widget-windows)).

All three views display information about the currently selected symbol:

!["Main Screen"](documentation/main_screen.png "Main Screen")

### 1. Search Field

The [Search Field](!interface#search-bar) allows for easy access to all indexed symbols. Use it to find all classes and functions you wish to investigate. In addition, it also holds the UI buttons for navigating [back & forward](!interface#back-&-forward) as well as [refreshing](!interface#refresh).

<img src="documentation/search_view.png" width="750" alt="Search View">

When entering a search query, the [autocompletion popup](!interface#autocompletion-popup) will provide you with a concise list of all matching symbols. Note that NumbatUI uses a fuzzy matching algorithm, that allows you to skip characters while entering a query.

<img src="documentation/search_view_completion.png" width="800" alt="Search View Completion">

### 2. Graph Visualization

The [graph visualization](!interface#graph-view) displays the currently selected symbol in an active state and all the other symbols it shares a relationship with. The visualization is made up of nodes and edges.

* **Nodes:** All named symbols in your source code will be displayed as different [nodes](!interface#nodes), such as `functions`, `classes` or `files`. Nodes with members (like `classes`) can be expanded to show all their contents, the number at the expansion arrow shows how many members are hidden. Clicking a node will activate it and update all the views to the new selection. Dragging a node can be used to change its position.
* **Edges:** The relationships between the symbols are displayed as different [edges](!interface#edges), such as `type use`, `function call` or `file include`. Sometimes edges get bundled together and are displayed as `bundled edges` that show a number of how many edges are contained. Clicking an edge will highlight its source location in the code view.

!["Graph View Graph"](documentation/graph_view_graph.png "Graph View Graph")

* **Colors:** The different node and edge types are also displayed using different colors. The default color scheme uses this convention:

    | Color | Node | Edge
    | --- | --- | ---
    | gray | types and classes | type use
    | yellow | functions and methods | calls
    | blue | variables and fields | variable access

* **Hatching:** Nodes displayed with a striped hatching, are nodes that were used within your indexed source files, but were not defined. Clicking them shows all locations where they are used, without providing their declaration.

    !["Node Non Indexed"](documentation/node_non_indexed.png "Node Non Indexed")

* **Legend:** For a full list on all existing nodes and edges take a look at the integrated [Graph Legend](!interface#graph-legend) by clicking the `?`-button in the bottom right corner of the [Graph View](!interface#graph-view).

### 3. Code View

The [code view](!interface#code-view) displays all locations of the currently active symbol within the indexed source files. It does not allow for editing the source code. Syntax highlighting is used to increase readability. Source locations that are surrounded by a box when hovered can be clicked to activate the respective symbol. Active source locations are highlighted.

<img src="documentation/code_view.png" width="800" alt="Code View">

The source locations are displayed as code snippets, containing the line of interest and extra lines added to the top and bottom to give information about its context. Code snippets are then bundled together into files.

**Note:** A file can be selected as active symbol by clicking its name in the title bar. By clicking the title bar or the icon on the right hand side of the title bar, the file can switch between 3 different states:

* **Minimized:** The file does not show any content

<img src="documentation/snippet_minimized.png" width="800" alt="Snippet Minimized">

* **Snippets:** The file displays the snippets containing active locations separated by lines.

<img src="documentation/snippet_snippets.png" width="800" alt="Snippet Snippets">

* **Maximized:** The [code view](!interface#code-view) switches to [single file mode](!interface#single-file-mode) and the whole content of the file is visible.

<img src="documentation/code_view_single.png" width="800" alt="Code View Single">

For more information, please visit the [Code View Files](!interface#files) section.

## Start Exploring!

At this point, you should have an understanding of the basics of NumbatUI's user interface and can begin exploring your codebase. NumbatUI will allow you to see your source code from a whole new perspective, by giving you a concise overview of its parts and a faster way of drilling down to its internals, while always maintaining the connection to the implementation details of the actual source code.

Please take look at the much more extensive instruction manual below for detailed information.

The NumbatUI team wishes you a good start with our product, lots of saved time, increased productivity and much cleaner code.

!!! example "Tasks"
    * Start exploring and have fun!
