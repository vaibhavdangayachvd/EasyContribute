/**
    Purpose: Easy Contribution (Linked List)
    #created 28/08/18 3:10AM

    @author Vaibhav As VD
    @version 1.0 (28/08/18 03:10AM)
    @version 1.2 (29/08/18 11:41PM)
    @version 1.3 (01/09/18 12:20AM)


    Change log :-
    v1 - Initial Release
       - Inter Group Payments
       - Club Entry Manager
       - Added Documentation
       - Can be compiled on turbo c by just changing type in macro define from 1 to 2
       Major Bugfix :- Wrong extra amount passed to distribute function.
       Minor Bugfix :- Reset was not working properly.
                     - Removed extra parameter in set entry function.
                     - Reset Fixed

  v1.2 - Minor and Major Bug Fixes
       - Crash Fixes
       - Better error handling
       - Reliability Changes
       - Allow space in names
       - Possibility Re-Calculate any number of times with different values of entry and group activity
       - Possible to see names of all members while typing name
       - Feature Add - Complete Edit Menu
                     - Change Bill Amount/Entry Fee
                     - Display User List
                     - Remove Group activity
                     - Add/Remove/Edit Members Info

  v1.3 - Feature Add - Display who has to pay whom and how much
       - Stack Implementation for payment feature
       - Added paid field to structure details
*/
//Preprocessor Directives
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
#define TYPE 1//1 for Code blocks and any other for turbo
#if TYPE == 1
#define CLS system("cls")
#else
#define CLS clrscr()
#endif // TYPE
//Structure of members
struct details
{
    char name[20];
    float per;
    float grp;
    float total;
    float paid;
    struct details *next;
};
typedef struct details node;
//Stack Structure
struct stack
{
    node *ptr;
    struct stack *next;
}*top=NULL;//Top
typedef struct stack st_node;
//Global Variables
float entry=0,per_tot=0,grp_tot=0,bill_amt=0,sub_grp_total=0;
int members = 0;
//UD Functions
void hold_screen(void);//Hold Screen
void create_list(node *head);//Create List
void set_entry_fee(void);//Set Entry Fee
void set_group_activity(node *head,int flag);//Set Inter group activity 1- add;0 - remove
node *find_user_node(node *head,char ch[]);//Find node from name
void calculate(node *head);//Calculate Result
void calculate_share(node *head,int count);//Calculate Each Persons Share
void distri_extra(node *head,float extra,int count);//Recursively Distribute Extra Amount
node*reset(node *head);//Reset Casino
void display_list(node *head,int per);//Display List with or without personal amt
//Edit Menu Functions
void open_edit_menu(node **head);//Open edit menu
void add_new_member(node *head);//Add Member
node *remove_member(node *head);//Remove Member
void edit_member_info(node *head);//Edit member info
//Stack Functions
void push(node *ptr);
void pop(void);
//Main program start
int main()
{
    node *head=NULL;
    char option;
    while(1)
    {
        CLS;
        printf("\t\t\tWelcome to Easy Conti by VD\n\n");
        printf("Choose Option :-\n1 - Create List\n2 - Display List\n3 - Set Entry Fee (optional)\n4 - Inter Group Payments (optional)\n5 - Open Edit Menu\n6 - Calculate Contribution\n7 - Reset\n\tBackspace - END\n");
        option = getch();
        CLS;
        switch(option)
        {
        case '1':
            if(head!=NULL)
                printf("List Already Present !!");
            else
            {
                printf("\t\t\tHere You Can Create List\n\n");
                printf("Enter number of people in your group : ");
                scanf(" %d",&members);
                printf("\n");
                if(members<=0)
                    printf("Wrong Amount !!");
                else
                {
                    head = (node*)malloc(sizeof(node));
                    create_list(head);
                }
            }
            hold_screen();
            break;
        case '2':
            if(head==NULL)
                printf("Create List First !!");
            else
            {
                printf("\t\t\tHere you can see details of members\n\n");
                printf("Member name followed by their personal expenditure :-\n\n");
                display_list(head,1);
            }
            hold_screen();
            break;
        case '3':
            if(head==NULL)
                printf("Create List First !!");
            else if(entry)
                printf("Entry Fee Already Set !! Goto Edit Menu to change");
            else
            {
                printf("\t\t\tHere You Can Set Entry Fee\n\n");
                set_entry_fee();
            }
            hold_screen();
            break;
        case '4':
            if(head==NULL)
                printf("Create List First !!");
            else
            {
                printf("\t\t\tHere You Can Set Group Activity\n\n");
                set_group_activity(head,1);
            }
            hold_screen();
            break;
        case '5':
            if(head==NULL)
            {
                printf("Create List First !!");
                hold_screen();
            }
            else
                open_edit_menu(&head);
            break;
        case '6':
            if(head==NULL)
                printf("Create List First !!");
            else
                calculate(head);
            hold_screen();
            break;
        case '7':
            printf("\n\n\n\n\n\n\n\t\t\t\tPress Enter to Reset or Backspace to go Back\n");
            option = getch();
            if(option != 8)
            {
                head = reset(head);
                entry = 0,per_tot=0,members=0,grp_tot=0,bill_amt=0,sub_grp_total=0;
                printf("\n\nReset Successfull !!");
                hold_screen();
            }
            break;
        case 8 :
            return 0;
        }
    }
}//Main End Here
void hold_screen()
{
    printf("\n\n\t\t\tPress Enter to continue...");
    getch();
}
void create_list(node *head)
{
    int i;
    for(i=0;i<members;++i)
    {
        printf("Enter name of member %d : ",i+1);
        scanf(" %[^\n]",head->name);
        head->per=0;
        printf("Enter personal expenditure of %s : ",head->name);
        scanf(" %f",&head->per);
        head->total=0;
        head->paid=0;
        head->grp=0;
        per_tot+=head->per;
        printf("\n");
        //If last member set next to null else allot memory
        if(i == members-1)
            head->next=NULL;
        else
            head->next = (node*)malloc(sizeof(node));
        head=head->next;
    }
    jump:
    printf("\tEnter Final Bill Amount : ");
    scanf(" %f",&bill_amt);
    if(bill_amt<per_tot)
    {
        printf("  Wrong Bill Amount !!\n");
        goto jump;
    }
    grp_tot=bill_amt-per_tot;//Set Group Total
    printf("\nList Successfully Created !!");
}
void set_entry_fee(void)
{
    printf("Enter entry fee (0 to continue without setting entry fee): ");
    jump:
    scanf(" %f",&entry);
    if(entry>bill_amt)
    {
        printf("Wrong Amount !!");
        goto jump;
    }
    else if(entry <= 0)
    {
        entry = 0;
        printf("Empty amount set !!");
    }
    else
    {
        //Treat entry fee as group activity
        grp_tot+=entry;
        printf("\n\t\tEntry Fee Included !!");
    }
}
void calculate(node *head)
{
    node *ptr=head;
    float extra=0.0;
    int count=0;
    //Add Group amt to grp balance of each
    while(ptr!=NULL)
    {
        ptr->grp+=(grp_tot/members);
        ptr=ptr->next;
    }
    ptr=head;
    //If No Entry Fee
    if(entry==0)
    {
        while(ptr!=NULL)
        {
            ptr->total = ptr->grp+ptr->per;
            ptr=ptr->next;
        }
    }
    //If Entry Fee
    else
    {
        while(ptr!=NULL)
        {
            //If someone spend less than the entry fee
            if(ptr->grp+ptr->per - entry/members <=entry/members)
            {
                ptr->total=entry/members;
                //Save its leftover balance
                extra += entry/members-(ptr->grp+ptr->per - entry/members);
                //Count number of members whose result is calculated and saved in total field
                count++;
            }
            ptr=ptr->next;
        }
        //Distribute Extra Balance in other members who spent more than entry
        distri_extra(head,extra,count);
    }
    //Print Result
    ptr = head;
    printf("\t\t\tShare of Each Member :- \n\n");
    while(ptr!=NULL)
    {
        printf("%s - Rs. %.2f\n",ptr->name,ptr->total);
        ptr->grp-=(grp_tot/members);
        ptr=ptr->next;
    }
    printf("\nEnter number of people who contributed in bill payment (0 to skip this step and exit): ");
    scanf("%d",&count);
    printf("\n");
    if(count<=0)
        return;
    else
        calculate_share(head,count);
    ptr=head;
    while(ptr!=NULL)
    {
        ptr->paid = 0;
        ptr->total= 0;
        ptr=ptr->next;
    }
}
void distri_extra(node *head,float extra,int count)
{
    node *ptr=head;
    float amount;
    int temp_count=count;
    while(ptr!=NULL)
    {
        amount = ptr->grp+ptr->per - entry/members - extra/(members-count);
        //Check if any extra balance left after giving extra
        if(ptr->total==0 && amount<= entry/members)
        {
            ptr->total=entry/members;
            extra-=extra/(members-count);//Reduce Extra Balance
            extra+=entry/members-amount;//Add leftover balance to extra
            //Count number of members whose result is calculated and saved in total field
            count++;
        }
        ptr=ptr->next;
    }
    //If not extra balance can be accumulated
    if(temp_count==count)
    {
        ptr=head;
        //Set Result of all by reducing accumulated extra balance from their balance
        while(ptr!=NULL)
        {
            amount = ptr->grp+ptr->per - entry/members - extra/(members-count);
            if(ptr->total==0)
            {
                ptr->total = amount;
            }
            ptr=ptr->next;
        }
        return;
    }
    //if balance is accumulated try again with updated extra balance and count
    distri_extra(head,extra,count);
}
void calculate_share(node *head,int count)
{
    int i;
    char ch[20];
    node *ptr;
    float amount;
    for(i=0;i<count;++i)
    {
        printf("Enter name of payer %d : ",i+1);
        scanf(" %[^\n]",ch);
        ptr=find_user_node(head,ch);
        if(ptr==NULL)
        {
            printf("User not found !! Try Again !!\n\n");
            --i;
            continue;
        }
        else
        {
            printf("Enter amount paid by %s : ",ptr->name);
            scanf("%f",&ptr->paid);
            printf("\n");
            if(ptr->paid>ptr->total)
                push(ptr);
        }
    }
    printf("Pay as follows :-\n\n");
    ptr=head;
    while(ptr!=NULL)
    {
        //If someone paid less than total or paid nothing
        if(ptr->paid < ptr->total)
        {
            //if extra payer on stack has nothing ext left
            if(top->ptr->paid-top->ptr->total==0)
                pop();
            //If extra payer can settle full leftover amount
            if(top->ptr->paid-top->ptr->total>=ptr->total-ptr->paid)
            {
                printf("%s pay %.2f to %s\n\n",ptr->name,(ptr->total-ptr->paid),top->ptr->name);
                top->ptr->paid-=(ptr->total-ptr->paid);
                ptr->paid=ptr->total;
                ptr=ptr->next;
            }
            //if extra payer can settle some portion of amount
            else
            {
                ptr->paid+=(top->ptr->paid-top->ptr->total);
                printf("%s pay %.2f to %s\n",ptr->name,(top->ptr->paid-top->ptr->total),top->ptr->name);
                top->ptr->paid-=(top->ptr->paid-top->ptr->total);
            }
        }
        else
            ptr=ptr->next;
    }
}
void push(node *ptr)
{
    st_node *temp=(st_node*)malloc(sizeof(st_node));
    temp->ptr=ptr;
    temp->next=top;
    top=temp;
}
void pop(void)
{
    st_node *temp;
    if(top==NULL)
        return;
    temp=top;
    top=top->next;
    free(temp);
}
node * reset(node *head)
{
    node *ptr=head;
    while(head!=NULL)
    {
        ptr=head;
        head=head->next;
        ptr->per=0,ptr->grp=0,ptr->total=0,ptr->paid=0;
        free(ptr);
    }
    return head;
}
node *find_user_node(node *head,char ch[])
{
    while(head != NULL && strcmp(head->name,ch))
        head=head->next;
    if(head==NULL)
        return NULL;
    else
        return head;
}
void set_group_activity(node *head,int flag)
{
    int num,i;
    float total;
    node *ptr;
    char ch[20];
    printf("Enter number of students in sub-group : ");
    scanf(" %d",&num);
    if(num<=0)
    {
        printf("No Amount Set !!");
        return;
    }
    printf("\nEnter Sub Group Amount : ");
    jump:
    scanf(" %f",&total);
    if(total>bill_amt && flag == 1)
    {
        printf("Wrong Amount !!");
        goto jump;
    }
    else if(total>sub_grp_total && flag == 0)
    {
        printf("Wrong Amount !!");
        goto jump;
    }
    printf("\nName of all members (for reference) :-\n\n");
    display_list(head,0);
    printf("\n");
    //Sub-Total is added while calculating group total so it is reduced
    if(flag)
    {
        grp_tot-=total;
        sub_grp_total+=total;
    }
    else
    {
        grp_tot+=total;
        sub_grp_total-=total;
    }
    total/=(float)num;
    //Adding total to their individual personal grp expenditure
    for(i=0;i<num;++i)
    {
        printf("Enter name of member %d : ",i+1);
        scanf(" %[^\n]",ch);
        ptr=find_user_node(head,ch);
        if(ptr==NULL)
        {
            printf("User not found !! Try Again !!\n\n");
            --i;
            continue;
        }
        else
        {
            if(flag)
                ptr->grp+=total;
            else
                ptr->grp-=total;
        }
    }
    if(flag)
         printf("\n\tGroup Activity Set !!");
}
void display_list(node *head,int per)
{
    while(head!=NULL)
    {
        //if personal amount needed
        if(per)
            printf("%s - %.2f\n",head->name,head->per);
        else
            printf("%s\n",head->name);
        head=head->next;
    }
}
void open_edit_menu(node **head)
{
    char option;
    while(1)
    {
        CLS;
        printf("\t\t\tHere you can edit different values\n\n");
        printf("Choose option :-\n\n");
        printf("1 - Add New Member\n2 - Remove existing member\n3 - Edit member info\n4 - Change Bill Amount\n5 - Change Entry Fee\n6 - Remove group activity\n\tBackspace = Main Menu\n");
        option = getch();
        CLS;
        switch(option)
        {
        case '1':
            add_new_member(*head);
            grp_tot=bill_amt-per_tot+entry-sub_grp_total;//Set New Group Total
            members++;
            printf("\nNew Member Added !!");
            hold_screen();
            break;
        case '2':
            *head = remove_member(*head);
            grp_tot=bill_amt-per_tot+entry-sub_grp_total;//Set New Group Total
            members--;
            printf("\nMember Removed !!");
            hold_screen();
            break;
        case '3':
            printf("\t\t\tHere you can edit member info\n\n");
            edit_member_info(*head);
            break;
        case '4':
            printf("\t\t\tHere you can set new bill amount\n\n");
            jump:
            printf("\tEnter New Final Bill Amount : ");
            scanf(" %f",&bill_amt);
            if(bill_amt<per_tot)
            {
                printf("  Wrong Bill Amount !!\n");
                goto jump;
            }
            grp_tot=bill_amt-per_tot+entry-sub_grp_total;//Set New Group Total
            printf("\nNew Bill Amount Set !!");
            hold_screen();
            break;
        case '5':
            if(entry==0)
                printf("No Entry Fee Is Set");
            else
            {
                printf("\t\t\tHere you can set new entry amount\n\n");
                set_entry_fee();
                grp_tot=bill_amt-per_tot+entry-sub_grp_total;//Set New Group Total
            }
            hold_screen();
            break;
        case '6':
            if(sub_grp_total<=0)
                printf("No existing group activity !!");
            else
            {
                printf("\t\t\tHere You Can Remove Group Activity\n\n");
                set_group_activity(*head,0);
                printf("\n\tGroup Activity Removed !!");
            }
            hold_screen();
            break;
        case 8:
            return;
        }
    }
}
void add_new_member(node *head)
{
    printf("\t\t\tHere you can add new member\n\n");
    node *temp = (node*)malloc(sizeof(node));
    printf("Enter name of member : ");
    scanf(" %[^\n]",temp->name);
    temp->per=0;
    printf("Enter personal expenditure of %s : ",temp->name);
    scanf(" %f",&temp->per);
    temp->total=0;
    temp->grp=0;
    temp->next=NULL;
    per_tot+=temp->per;
    while(head->next!=NULL)
        head=head->next;
    head->next=temp;
}
node *remove_member(node *head)
{
    char ch[20];
    node *ptr=NULL,*prev=head;
    printf("\t\t\tHere you can remove existing member\n\n");
    printf("Name of all members (for reference) :-\n\n");
    display_list(head,0);
    printf("\n");
    while(ptr==NULL)
    {
        printf("Enter name of member : ");
        scanf(" %[^\n]",ch);
        ptr=find_user_node(head,ch);
        if(ptr==NULL)
            printf("User not found !! Try Again !!\n\n");
    }
    per_tot-=ptr->per;
    while(prev->next!=ptr)
        prev=prev->next;
    if(ptr==head)
        head=head->next;
    else
        prev->next=ptr->next;
    free(ptr);
    return head;
}
void edit_member_info(node *head)
{
    char ch[20],option;
    node *ptr=NULL;
    printf("Name and personal amount of all members (for reference) :-\n\n");
    display_list(head,1);
    printf("\n");
    while(ptr==NULL)
    {
        printf("Enter name of member : ");
        scanf(" %[^\n]",ch);
        ptr=find_user_node(head,ch);
        if(ptr==NULL)
            printf("User not found !! Try Again !!\n\n");
    }
    while(1)
    {
        CLS;
        printf("\t\t\tHere you can edit member info\n\n");
        printf("Choose Option :-\n\n");
        printf("1 - Edit Name\n2 - Edit Personal Amount\n\tBackspace - Edit Menu\n");
        option = getch();
        CLS;
        switch(option)
        {
        case '1':
            printf("\t\t\tHere you can edit member name\n\n");
            printf("Old Name : %s\n",ptr->name);
            printf("Enter New Name : ");
            scanf(" %[^\n]",ptr->name);
            printf("\nName Changed !!");
            hold_screen();
            break;
        case '2':
            printf("\t\t\tHere you can edit member personal expenditure amount\n\n");
            per_tot-=ptr->per;
            printf("Old  personal expenditure of %s : %f\n",ptr->name,ptr->per);
            printf("Enter new personal expenditure of %s : ",ptr->name);
            scanf(" %f",&ptr->per);
            per_tot+=ptr->per;
            grp_tot=bill_amt-per_tot+entry-sub_grp_total;//Set New Group Total
            printf("\nPersonal expenditure amount changed !!");
            hold_screen();
            break;
        case 8:
            return;
        }
    }
}
