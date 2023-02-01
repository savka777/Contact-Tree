#include <iostream>
#include<list>
#include<deque>
#include<string.h>
#include<fstream>
using namespace std;
typedef string String;


struct TreeNode
{
    String medicareId;          // unique identifier cannot be duplicated
    int directContacts = 0;     // no. of children a node has, default 0
    int totalCases = 1;         // no. of nodes rooted at this tree node including self
    TreeNode* parentPtr;        // pointer to the parent node contact->prev
    list<TreeNode*> directContactsPtrList; // list of pointers to children nodes
    TreeNode(const String& medId) : medicareId(medId), parentPtr(nullptr) {};

    TreeNode(TreeNode* nodePtr, const String& medId) : medicareId(medId), parentPtr(nodePtr) {};

};

class ContactTree
{
private:
    TreeNode* root;
public:
    ContactTree() { root = nullptr; }
    ~ContactTree() { DeleteContact(root->medicareId); }
    fstream file;
    bool isEmpty()
    {
        return root == nullptr;
    }

    int getSize()
    {
        //no.of nodes rooted at this tree node including self
        return root->totalCases;
    }

    void AddPatient0(const string& medId)
    {
        root = new TreeNode(medId);
    }

    void AddContact(const String& medId1, const String& medId2)
    {
        //medId1 is the parent of medId2
        //medId2 is the child of medID1

        //locating medId 1 in the tree using LookUpContact(contact medId1)
        TreeNode* contact = SearchContact(medId1);
        if (contact == nullptr) return; // if we dont find contact 

       // If found, create a new node with contact Id2 and add its location to node1’s directContactsList.
        TreeNode* newNode = new TreeNode(contact, medId2); // using second TreeNode contructor
        contact->directContactsPtrList.push_back(newNode);
        contact->directContacts++; //Increment parent’s direct contacts

        if (newNode->parentPtr != nullptr) { // going up the tree
            TreeNode* temp = newNode->parentPtr;
            do { //do at least once, increment other ancestors,  number of total cases.
                temp->totalCases++;
                temp = temp->parentPtr; //iterate over to next parent node
            } while (temp != nullptr); //check if condition is true
        }
    }

    TreeNode* SearchContact(const String& medId)
    {
        TreeNode* contact = lookUpContact(medId);
        if (contact == nullptr) // contact was not found
        {
            cout << "No contact has been found or has been deleted" << endl;
            return nullptr;
        }
        return contact;
    }

    TreeNode* lookUpContact(const String& medId)
    {
        if (isEmpty()) return nullptr; // if root == nullptr

        deque<TreeNode*>dq;

        //Start at the root.Push the root at the back of the deque.
        dq.push_back(root);

        while (!dq.empty())  //While the deque is not empty,
        {
            TreeNode* temp = dq.front();
            dq.pop_front();

            //check the front element, if it points to the node you seek, return it // best case
            if (temp->medicareId == medId) return temp;

            //otherwise, we iterate over the children of poped elemenet
            //https://www.cplusplus.com/reference/list/list/begin/

            list<TreeNode*>::iterator iterator;
            for (iterator = temp->directContactsPtrList.begin(); iterator != temp->directContactsPtrList.end(); iterator++)
            {
                //If not, add its direct contact pointers to the end of the deque
                dq.push_back(*iterator);
            }
        }
        return nullptr; // contact not found
    }

    void Print(TreeNode* node)
    {
        cout << " " << "---------TRACING INFORMATION---------" << endl;
        cout << " " << "Medical ID: " << node->medicareId << endl;
        cout << " " << "Direct Contacts : " << node->directContacts << endl;
        cout << " " << "Total Cases: " << node->totalCases << endl;

        file.open("Info.txt", fstream::in | ::fstream::out | fstream::app);
        file << "---------TRACING INFORMATION---------\n";
        file << "Medical ID: " << node->medicareId << "\n";
        file << "Direct Contacts : " << node->directContacts << "\n";
        file << "Total Cases: " << node->totalCases << "\n";
        file.close();

    }
    void DisplayContact(TreeNode* node)
    {
        Print(node);
    }

    void DisplayContact(const String& medId)
    {
        //search for contact, if found print data
        TreeNode* contact = SearchContact(medId);
        if (contact == nullptr) return;
        DisplayContact(contact);
    }

    void TraceSource(const String& medId) //recursive 
    {
        TreeNode* contact = SearchContact(medId);
        if (contact == nullptr) return;

        DisplayContact(contact);
        TraceSource(contact->parentPtr->medicareId);
    }

    void PrintContactCases(const String& medId)
    {
        TreeNode* contact = SearchContact(medId);
        if (contact == nullptr) return;

        //https://www.cplusplus.com/reference/list/list/begin/
        list<TreeNode*>::iterator iterator;
        for (iterator = contact->directContactsPtrList.begin(); iterator != contact->directContactsPtrList.end(); ++iterator) {
            DisplayContact(*iterator);
        }
    }

    void PrintContactTree()
    {
        //An iterative Breadth First Traversal that works the same way as the LookUpContact method.Here,
        // print the node’s data.You may find a call to DisplayContact(TreeNode*) useful

        if (isEmpty()) return; //terminate
        deque<TreeNode*> dq;

        dq.push_back(root);

        while (!dq.empty()) {
            TreeNode* temp = dq.front();
            dq.pop_front();

            DisplayContact(temp);

            list<TreeNode*>::iterator iterator;
            for (iterator = temp->directContactsPtrList.begin(); iterator != temp->directContactsPtrList.end(); ++iterator)
            {
                dq.push_back(*iterator);
            }
        }
    }

    void DeleteContact(const String& medId)
    {
        //using searchContact to return the contact we are looking for, or return null 
        TreeNode* contact = SearchContact(medId);
        if (contact == nullptr) return; //terminate

        deque<TreeNode*> toDeleteCol; // a collection of pointers that will be deleted 
        deque<TreeNode*> dq; //main deque where the delete of the collection of pointers will happen
        toDeleteCol.push_back(contact);

        while (!toDeleteCol.empty())
        {   //temp pointer to access the front of the deque
            TreeNode* temp = toDeleteCol.front();
            toDeleteCol.pop_front();
            //pushed into main dequeu
            dq.push_back(temp);

            //pushed all the children
            list<TreeNode*>::iterator iterator; //push all the children
            for (iterator = temp->directContactsPtrList.begin(); iterator != temp->directContactsPtrList.end(); ++iterator)
            {
                toDeleteCol.push_back(*iterator);
            }
            /*
            delete(temp);
            toDeleteCol.pop_front();
            */
        }
        //we now have all the Tree Nodes* associated with the sub tree of the contact we wish to delete 
        while (!dq.empty())
        {
            TreeNode* currentElement = dq.back(); //last element
            dq.pop_back(); // pop the last element

            if (currentElement == contact) //compare the element in this deque to see if it is the root of the sub tree
                //if not we delete and continue, until the deque is empty,
                /*When you arrive at that subtree’s root, make sure you delete its pointer from the parents’ list
                of children. Don’t forget to decrement the parents’ direct contact as well each of the ancestors’
                total cases*/
            {
                
                currentElement->parentPtr->directContacts -= currentElement->directContacts; // decrement no. children a node has
                currentElement->parentPtr->directContactsPtrList.remove(currentElement); //remove method is used to remove from the container, and reduce the size https://www.cplusplus.com/reference/list/list/remove/

                //Delete the acestor's total cases 
                if (currentElement->parentPtr != nullptr) //iterate up , while the current nodes parent is not null
                {
                    TreeNode* tempParentPointer = currentElement->parentPtr;
                    do {
                        tempParentPointer->totalCases -= currentElement->totalCases; //decrments the total cases 
                        tempParentPointer = tempParentPointer->parentPtr; // t->next
                    } while (tempParentPointer != nullptr); //check condition // repeat

                }
            }
            delete currentElement; // delete current
        }
    }
};

int main()
{

    ContactTree ct;
    ct.AddPatient0("Savva");
    ct.AddContact("Savva", "Michelle");
    ct.AddContact("Savva", "John");
    ct.AddContact("Savva", "Juan");
    //----------------------------------
    ct.AddContact("Michelle", "Kevin");
    ct.AddContact("Michelle", "Kiril");
    ct.AddContact("Michelle", "Michael");
    //------------------------------------
    ct.AddContact("Kevin", "Monica");
    ct.AddContact("Kevin", "Patrick");
    //------------------------------------
    ct.AddContact("Monica", "Shuyuan");
    ct.AddContact("Shuyuan", "Ema");
    ct.AddContact("Ema", "Zoran");
    ct.PrintContactTree();
}