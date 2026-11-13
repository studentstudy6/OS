#!/bin/bash
# Menu driven Address Book  (record format: name|phone|email)
ADDRESSBOOK="addressbook.txt"
while true
do
    echo "================================="
    echo "        ADDRESS BOOK MENU"
    echo "================================="
    echo "1. Create Address Book"
    echo "2. View Address Book"
    echo "3. Insert a Record"
    echo "4. Delete a Record"
    echo "5. Modify a Record"
    echo "6. Exit"
    echo "================================="
    read -r -p "Enter your choice: " choice
    case "$choice" in
    1)
        touch "$ADDRESSBOOK"
        echo "Address Book Created Successfully."
        ;;
    2)
        if [ -f "$ADDRESSBOOK" ]
        then
            echo "----- Address Book Records -----"
            cat "$ADDRESSBOOK"
        else
            echo "Address Book does not exist."
        fi
        ;;
    3)
        if [ ! -f "$ADDRESSBOOK" ]
        then
            echo "Address Book does not exist. Create it first (option 1)."
        else
            read -r -p "Enter Name: " name
            read -r -p "Enter Phone Number: " phone
            read -r -p "Enter Email: " email
            echo "$name|$phone|$email" >> "$ADDRESSBOOK"
            echo "Record Inserted Successfully."
        fi
        ;;
    4)
    5)
    6)
    *)
        if [ ! -f "$ADDRESSBOOK" ]
        then
            echo "Address Book does not exist."
        else
            read -r -p "Enter Name to Delete: " name
            if grep -i -q "^$name|" "$ADDRESSBOOK"
            then
                grep -i -v "^$name|" "$ADDRESSBOOK" > temp.txt
                mv temp.txt "$ADDRESSBOOK"
                echo "Record Deleted Successfully."
            else
                echo "Record Not Found."
            fi
        fi
        ;;
        if [ ! -f "$ADDRESSBOOK" ]
        then
            echo "Address Book does not exist."
        else
            read -r -p "Enter Name to Modify: " name
            if grep -i -q "^$name|" "$ADDRESSBOOK"
            then
                grep -i -v "^$name|" "$ADDRESSBOOK" > temp.txt
                read -r -p "Enter New Name: " newname
                read -r -p "Enter New Phone Number: " newphone
                read -r -p "Enter New Email: " newemail
                echo "$newname|$newphone|$newemail" >> temp.txt
                mv temp.txt "$ADDRESSBOOK"
                echo "Record Modified Successfully."
            else
                echo "Record Not Found."
            fi
        fi
        ;;
        echo "Exiting Program..."
        exit 0
        ;;
        echo "Invalid Choice. Try Again."
        ;;
    esac
    echo
done
