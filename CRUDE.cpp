#include <iostream>
using namespace std;

int main (){
    int operation;
    int id[100], n = 0, i, idSearch, isSearch;
    string name[100], gender[100], nameSearch, textYesNo;
    float salary[100];
    
    do {
        cout << "------ CRUD SYSTEM ------" << '\n';
        cout << "1. Input staff" << '\n';
        cout << "2. List staff " << '\n';
        cout << "3. Search staff by id " << '\n';
        cout << "4. Search staff by name " << '\n';
        cout << "5. Update staff " << '\n';
        cout << "6. Delete staff " << '\n';
        cout << "7. Exit " << '\n';
        cout << "Input Choice: ";
        cin >> operation;
        
        switch (operation) {
            case 1:
                cout << "------ Input Staff Information ------" << '\n';
                cout << "Input staff id: ";
                cin >> id[n];
                cout << "Input staff name: ";
                cin >> name[n];
                cout << "Input staff gender: ";
                cin >> gender[n];
                cout << "Input staff salary: "; 
                cin >> salary[n];
                n++;
                break;
            
            case 2:
                cout << "------ List Staff ------" << '\n';
                for (i = 0; i < n; i++) {
                    cout << "ID: " << id[i] << '\t';
                    cout << "Name: " << name[i] << '\t';
                    cout << "Gender: " << gender[i] << '\t';
                    cout << "Salary: " << salary[i] << endl;
                }
                break;
                
            case 3:
                cout << "------ Search Staff By ID ------" << '\n';
                cout << "Input the staff ID to search: ";
                cin >> idSearch;
                isSearch = 0;
                for (i = 0; i < n; i++) {
                    if (idSearch == id[i]) {
                        cout << "------- STAFF FOUND -------" << '\n';
                        cout << "ID: " << id[i] << '\t';
                        cout << "Name: " << name[i] << '\t';
                        cout << "Gender: " << gender[i] << '\t';
                        cout << "Salary: " << salary[i] << endl;
                        isSearch = 1;
                        break;
                    }
                }
                if (isSearch == 0) {
                    cout << idSearch << " not found in the list" << '\n';
                }
                break;
                
            case 4:
                cout << "------ Search Staff By Name ------" << '\n';
                cout << "Enter the staff name to search: ";
                cin >> nameSearch;
                isSearch = 0;
                for (i = 0; i < n; i++) {
                    if (nameSearch == name[i]) {
                        cout << "------ Staff Found ------" << '\n';
                        cout << "ID: " << id[i] << '\t';
                        cout << "Name: " << name[i] << '\t';
                        cout << "Gender: " << gender[i] << '\t';
                        cout << "Salary: " << salary[i] << '\n';
                        isSearch = 1;
                        break;
                    }
                }
                if (isSearch == 0) {
                    cout << nameSearch << " Not found in the list" << '\n';
                }
                break;
                
            case 5:
                cout << "------ Update Staff Information ------" << '\n';
                cout << "Input staff ID to update: ";
                cin >> idSearch;
                isSearch = 0;
                for (i = 0; i < n; i++) {
                    if (idSearch == id[i]) {
                        // IF THE STAFF IS FOUND
                        cout << "------- Staff Information Found ------" << '\n';
                        cout << "ID: " << id[i] << '\t';
                        cout << "Name: " << name[i] << '\t';
                        cout << "Gender: " << gender[i] << '\t';
                        cout << "Salary: " << salary[i] << '\n';
                        // UPDATE THE NEW DATA
                        cout << "Input New Name: ";
                        cin >> name[i];
                        cout << "Input New Gender: ";
                        cin >> gender[i];
                        cout << "Input New Salary: ";
                        cin >> salary[i];
                        cout << "------ Information Updated ------" << '\n';
                        isSearch = 1;
                        break;
                    }
                }
                if (isSearch == 0) {
                    cout << "ID IS NOT FOUND IN THE LIST." << '\n';
                }
                break;
                
            case 6:
                cout << "------ Delete Staff By ID ------" << '\n';
                cout << "Input the staff ID to delete: ";
                cin >> idSearch;
                isSearch = 0;
                for (i = 0; i < n; i++) {
                    if (idSearch == id[i]) {
                        // SHOW STAFF INFORMATION
                        cout << "------ STAFF FOUND ------" << '\n';
                        cout << "ID: " << id[i] << '\t';
                        cout << "Name: " << name[i] << '\t';
                        cout << "Gender: " << gender[i] << '\t';
                        cout << "Salary: " << salary[i] << '\n';
                        cout << endl;

                        // Ask for confirmation to delete
                        cout << "Confirmation to delete the staff? (yes/no): ";
                        cin >> textYesNo;
                        
                        if (textYesNo == "yes" || textYesNo == "Yes") {
                            // PROCEEDS TO DELETE
                            for (int j = i; j < n - 1; j++) {
                                id[j] = id[j + 1];
                                name[j] = name[j + 1];
                                gender[j] = gender[j + 1];
                                salary[j] = salary[j + 1];
                            }
                            n--; // Decrement total number of staff
                            cout << "------ Delete Successful ------" << '\n';
                        } else {
                            cout << "Deletion cancelled.\n";
                        }
                        isSearch = 1;
                        break;
                    }
                }
                if (isSearch == 0) {
                    cout << "ID IS NOT FOUND IN THE LIST" << '\n';
                }
                break;
                
            case 7:
                return 0; 
                break;
        }
    } while (operation != 7);
    
    return 0;
}
