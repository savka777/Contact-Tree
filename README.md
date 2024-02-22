# Contact Tracing Program

This C++ program is designed to simulate a contact tracing system. The primary purpose is to track and manage the spread of  diseases by monitoring direct and indirect contacts among individuals. The program uses a tree data structure to represent the relationships between contacts.

Features <br>
Tree Structure: Utilizes a tree to represent contact relationships, where each node is a person, identified by a unique Medicare ID. <br>
Contact Tracing: Allows tracing of direct and indirect contacts of an infected person. <br>
Dynamic Contact Management: Provides functions to add new contacts, trace the source of infection, and delete contacts. <br>
Data Output: Information about contacts is displayed on the console and written to a text file for record-keeping. <br>

Key Components <br>
TreeNode Structure: Represents an individual, storing their Medicare ID, direct contacts, total cases linked to them, and pointers to parent and children nodes. <br>
ContactTree Class: Manages the contact tracing tree, supporting operations like adding contacts, searching for contacts, printing contact information, and deleting contacts.

Functionality <br>
Add Contacts: Add a primary patient (Patient Zero) and subsequent contacts, establishing parent-child relationships in the tree. <br>
Search and Trace Contacts: Locate individuals in the tree and trace their direct and indirect contacts. <br>
Print Information: Display the contact information of an individual, including the total number of cases linked to them, either on the console or in a text file. <br>
Delete Contacts: Remove a contact from the tree, adjusting the tree structure and updating related nodes accordingly.
