# Doxygen Style Guide

## Introduction
This document is a style guide to provide best practice formatting guidelines for ALEX Software Documentation using Doxygen. As such, the guide is a compilation of material from the [Doxygen Manual](http://www.doxygen.nl/manual/docblocks.html) and the [How to Write Doc Comments for the Javadoc Tool](https://www.oracle.com/technical-resources/articles/java/javadoc-tool.html) technical article. The latter document contains a Javadoc style guide which provides conventions for creating useful API documents - these conventions are included at the end of this document as an essential documentation reference.

## Document Quick Links

1. [Setup for Commenting Using Doxygen](#h_setup_for_doxygen)
2. [Common Structure of Documentation Blocks](#h_common_struct)
3. [Documenting Classes](#h_doc_class)
4. [Documenting Enumerated Types](#h_doc_enum)
5. [Documenting Function/Method Definitions](#h_doc_func)
6. [Additional (Useful) Special Commands](#h_special_cmds)
7. [Tips From The Javadoc Style Guide](#h_javadoc)


## Setup for Commenting Using Doxygen<a name="h_setup_for_doxygen"></a>

### Doxygen Configuration File
The following tag values should be changed as shown below within the Doxyfile (Doxygen Configuration File) to produce correctly formatted Doxygen documentation:

```c++
CPP_CLI_SUPPORT = YES
JAVADOC_AUTOBRIEF = NO
EXTRACT_ALL = YES
GENERATE_TODOLIST = YES
CLASS_DIAGRAMS = YES
HAVE_DOT = YES
CLASS_GRAPH = YES
COLLABORATION_GRAPH = YES
UML_LOOK = YES
```

### Visual Studio Code
Note: Ensure the [Doxygen Document Generator](https://marketplace.visualstudio.com/items?itemName=cschlosser.doxdocgen) extension is installed and enabled in Visual Studio Code.

**Doxygen Document Generator** is used to automatically generate doc comment block templates. By default, it uses javadoc commands (prefixed by `@`). This prefix should be changed to the Doxygen standard prefix (`\`). Follow the steps below to make this change in Visual Studio Code:

In Visual Studio Code,
1. Navigate to **Settings** - **Extensions** - **Doxygen Document Generator**
2. Change all template prefixes from @ to \ as shown in the example below for the **Brief Template**

```c++
\brief{text} //Template prefix changed from @brief to \brief
```

## Common Structure of Documentation Blocks<a name="h_common_struct"></a>

### Brief Description<a name="brief_desc"></a>
A brief description is the first sentence of a doc comment block and should be short and concise but complete enough to describe the API item - ideally it should fit onto one line.

The`\brief` command starts a brief description paragraph. Brief descriptions are ended when a blank line is encountered. If several `\brief` descriptions are used in a comment block they will be joined.

### Detailed Description<a name="detailed_desc"></a>

A Detailed description is optional and begins after a blank line following a `\brief` command. It should provide further clarity and support to the brief description. Ideally it should be complete enough for conforming implementors and include enough description so that someone reading the source code can write a substantial suite of conformance tests. Where appropriate, include boundary conditions, parameter ranges and corner cases (special case scenarios)

```c++
/**
 * \brief This a brief description
 *
 * This is an optional detailed description to support the brief description above
 *
 */
```

### Parameters<a name="pars"></a>

`\param` commands are automatically created for each function argument when a doc comment block is triggered above the associated function (using `/** + Enter` when **Doxygen Document Generator** is enabled). The following tips (sourced from the [Javadoc Style Guide](https://www.oracle.com/technical-resources/articles/java/javadoc-tool.html)) provide convention for using `\param`:

* The first noun in the description is the data type of the parameter (may be preceded by an article "an", "a", "the"). Exception is the primitive type **int**
* Parameter names are lowercase

### Returns<a name="rets"></a>

A `\return` command is automatically created for each function which returns a value when the doc comment block is triggered above the associated function (using `/** + Enter` when **Doxygen Document Generator** is enabled). The following tips (sourced from the [Javadoc Style Guide](https://www.oracle.com/technical-resources/articles/java/javadoc-tool.html)) provide convention for using `\return`:

*   **Omit** `\return` for methods that return void and for constructors
*   **Include** `\return` for all other methods, even if its content is entirely redundant with the method description
    *   Having an explicit `\return` tag makes it easier for someone to find the return value quickly
    *   Whenever possible, supply return values for special cases (such as specifying the value returned when an out-of-bounds argument is supplied)

### See Also<a name="sas"></a>

The `\sa` or `\see` command starts a paragraph which cross-references classes, functions, methods, variables or URLs. This optional command may be used to avoid repetition of the same information applicable across several class entities.

### Notes<a name="notes"></a>

The `\note` command starts a noteworthy paragraph. It is an optional command that can be used to draw emphasis to an important set of instructions within a doc comment block.

### Tables<a name="tables"></a>

Doxygen supports the use of Markdown to create tables in the doc comment blocks. This optional feature should be used when it is suitable to represent information in tabular form, such as key-value pairs which are not of an `enum` type. Please see the example below:

```c++
/**
 * First Header | Second Header
 * ------------ | -------------
 * Content Cell | Content Cell
 * Content Cell | Content Cell
 */
 ```

 For column alignment, one or two colons (`:`) can be used to select the type of alignment in the header seperator line.
 See the example below:
 ```c++
 /**
  * | Right | Center | Left |
  * | ----: | :----: | :----|
  * | value | value  | value|
  */
```

## Documenting Classes<a name="h_doc_class"></a>

Class doc comment blocks are placed before the class definition using two comment blocks. The first is at the start of the header file and contains the metadata of the class (`\file`, `\author`, `\date`, `\version`, and `\copyright` information) along with a description of the class. The second is placed immediately before the start of the class definition to provide an overall description of the class.

**Please Note**: The descriptions in both doc comment blocks may be identical as they appear in different places in the created Doxygen documentation, the documented header file and class definition, respectively. 

These doc comment block use the following structural elements:

1. [Brief Description](#brief_desc)
2. [Detailed Description](#detailed_desc) (recommended)
3. [Notes](#notes) (optional)

**Example of Header Doc Comment Block**

```c++
/**
 * \brief The %Drive class is used to interface with a CANOpen motor drive. According to the CiA402 standard
 * 
 * This class enables low level functions to the system. It does limited error
 * checking.
 *
 * \file Drive.h
 * \author Justin Fong
 * \date 2020-04-07
 * \version 0.1
 * \copyright Copyright (c) 2020
 *
 */
```

**Example of Class Definition Doc Comment Block**

```c++
/**
 * \ingroup Robot
 * \brief Abstract class describing a Drive used to communicate with a CANbus device. Note that many functions are implemented according to the CiA 402 Standard (but can be overridden)
 * 
 */
class Drive {
```

## Documenting Enumerated Types<a name="h_doc_enum"></a>

**Using In-line Comments to Display an Enum Table**
`/**< enum value 1 */` represents an inline comment. This comment is placed after the member it refers to, this association is indicated by `<` symbol in the comment description. Here, the placement of in-line comments associates the enum variable `Eval1` with its `value 1` and Doxygen creates an enum table from these associations.

```c++
/**
 * \brief Supported drive control modes
 * 
 */
enum ControlMode {
    UNCONFIGURED = 0,     /**< 0 */
    POSITION_CONTROL = 1, /**< 1 */
    VELOCITY_CONTROL = 2, /**< 2 */
    TORQUE_CONTROL = 3,   /**< 3 */
    ERROR = -1            /**< -1 */
};
```

[Doxygen Output - Documenting Enum Type](http://www.doxygen.nl/manual/examples/afterdoc/html/class_afterdoc___test.html)

## Documenting Function/Method Definitions<a name="h_doc_func"></a>

Function/Method definition doc comment blocks are place before the function/method definition. This doc comment block consists of the following structural elements:

1. [Brief Description](#brief_desc)
2. [Detailed Description](#detailed_desc) (recommended)
3. [Parameters](#pars) (if applicable)
4. [Returns](#rets) (if applicable)
5. [See Also](#sas) (optional)
6. [Notes](#notes) (optional)
7. [Todo](#todos) (optional)

**Example of Function/Method Doc Comment Block**

```c++
/**
 * \brief Generates the list of SDO commands required to configure position control in CANopen motor drive
 * 
 * \note More details on params and profiles can be found in the CANopne CiA 402 series specifications:
 *           https://www.can-cia.org/can-knowledge/canopen/cia402/
 * 
 * \param positionProfile describing motorProfile parameters for position control
 * \return std::vector<std::string> representing a generated list of SDO configuration commands for position control
 * \sa motorProfile
 */

std::vector<std::string> generatePosControlConfigSDO(motorProfile positionProfile);
```

## Additional (Useful) Special Commands <a name="h_special_cmds"></a> 
<a name="todos"></a> 

| Function Command | Decription | Doxygen Manual Link |
|----|----|----|
|`\code` | Starts a code block. A code block is interpreted as source code. Names of class, members, and other documented entities are replaced by links to the documentation. Use with `\endcode` | [\code command](http://www.doxygen.nl/manual/commands.html#cmdcode) |
|`\b<word>` | Displays the argument \<word> in bold font. Equivalent to HTML `<b>multiple words</b>` | [\b command](http://www.doxygen.nl/manual/commands.html#cmdb) |
|`\e<word>` | Displays the argument \<word> in italic font. Equivalent to HTML `<em>multiple words</em>` | [\e command](http://www.doxygen.nl/manual/commands.html#cmde) |
|`\details` | Starts the detailed description. Alternatively, a detailed description is started with a new paragraph (after a blank line) | [\detail command](http://www.doxygen.nl/manual/commands.html#cmddetails) |
|`\todo`| Starts a paragraph where a TODO item is described. The description will also add an item to a separate TODO list. | [\todo command](http://www.doxygen.nl/manual/commands.html#cmdtodo) |
|`\test` | Starts a paragraph where a test case can be described. The description will also add the test case to a separate test list. | [\test command](http://www.doxygen.nl/manual/commands.html#cmdtest) | 
|`\bug`|
|`\deprecated`|

## Tips from The Javadoc Style Guide<a name="h_javadoc"></a> 

Source: [Javadoc Style Guide](https://www.oracle.com/technical-resources/articles/java/javadoc-tool.html)
The following tips from the Javadoc Style Guide are presented as either one-to-one translation from the guide (see source reference above) or a paraphrase of the original content.
1. **First Sentence** - The first sentence of each doc comment should be a summary sentence, containing a concise but complete description of the API item
2. **Comment Description** - Ideally, make it complete enough for conforming implementors. Realistically, include enough description so that someone reading the source code can write a substantial suite of conformance tests. Basically, the spec should be complete, including boundary conditions, parameter ranges and corner cases
3. **Use \<code> style for keywords and names**
4. **Use in-line links economically**
    * It is not necessary to add links for all API names in a doc comment. Add API links when:
        *   The user might actually want to click on it for more information
        *   It is the first occurrence of the API name in the doc comment
5. **Omit parentheses for the general form of methods and constructors** - only use when referring to the specific form (for example, when you wish to distinguish between two methods)
6. **OK to use phrases instead of complete sentences, in the interest of brevity**
7. **Use 3rd person (Descriptive) not 2nd person (prescriptive)** - Gets the label (preferred) over Get the label (avoid)
8. **Method descriptions begin with a verb phrase** - A method implements an operation, or performs an action, so it should begin with a verb phrase (for example, This method sends a position command to the `Drive` described by DriveID)
9. **Class/interface/field descriptions can omit the subject and simply state the object** - The aformentioned API often describe things rather than actions or behaviours, as such: *Drive class containing*... (preferred) over *This is a Drive class containing* (avoid)
10. **Use "this" instead of "the" when referring to an object created from the current class.** - Gets the toolkit for **this** component (preferred) over Gets the toolkit for **the** component (avoid)
11. **Add description beyond the API name** - The best API names are "self-documenting". Thus, it is not beneficial to simply repeat the API name in description. The ideal comment should reward the reader with additional information which is not immediately obvious from the API name.
    **Avoid** - The description below says nothing beyond what you know from reading the method name. The words "set", "tool", "tip", and "text" are simply repeated in the sentence
    ```java
    /**
     * Sets the tool tip text.
     * 
     * @param text the text of the tool tip
     */
     public void setToolTipText(String text)
    ```
    **Preferred** - This description more completely defines what a tool tip is, in the larger context of registering and being displayed in response to the cursor.
    
    ```java
    /**
     * Registers the text to display in a tool tup. The text
     * displays when the cursor lingers over the component.
     *
     * @param text  the string to display. If the text is null,
                    the tool tip is turned off for this component.
     */
     public void setToolTipText(String text)
    ```
12. **Tag Conventions** - **Note:** The equivalent Doxygen commands will be used in place of Javadoc commands (@ replaced by \\)
    Order of Tags - Include tags in the following order
    1.  `\author` (classes and interfaces only, required)
    2.  `\version` (classes and interfaces only, required)
    3.  `\param` (methods and constructors only)
    4.  `\return` (methods only)
    5.  `\exception`(`\throws` is a synonym)
    6.  `\sa` or `\see\`
