// Functions for Sorting
#ifndef SORTED
#define SORTED
#include "Auxiliary.h"
int Sorted_CreateFile( char *fileName
/* όνομα αρχείου */ );
/*Η συνάρτηση ​ Sorted_CreateFile χρησιμοποιείται για τη δημιουργία και κατάλληλη αρχικοποίηση ενός
άδειου αρχείου σωρού με όνομα ​ fileName. Σε περίπτωση που εκτελεστεί επιτυχώς, επιστρέφεται 0, ενώ
σε διαφορετική περίπτωση -1.*/

int Sorted_OpenFile( char *fileName
/* όνομα αρχείου */ );
/*Η συνάρτηση ​ Sorted_OpenFile ανοίγει το αρχείο με όνομα ​ fileΝame και διαβάζει από το πρώτο μπλοκ
την πληροφορία που αφορά στο αρχείο. Επιστρέφει τον αναγνωριστικό αριθμό ανοίγματος αρχείου, όπως
αυτός επιστράφηκε από το επίπεδο διαχείρισης μπλοκ ή -1 σε περίπτωση σφάλματος. Αν το αρχείο που
ανοίχτηκε δεν πρόκειται για ταξινομημένο αρχείο σωρού, τότε αυτό θεωρείται επίσης περίπτωση
σφάλματος.*/

int Sorted_CloseFile( int fileDesc
/* αναγνωριστικός αριθμός ανοίγματος αρχείου */ );
/*Η συνάρτηση ​ Sorted_CloseFile κλείνει το αρχείο που προσδιορίζεται από τον αναγνωριστικό αριθμό
ανοίγματος ​ fileDesc. Σε περίπτωση που εκτελεστεί επιτυχώς, επιστρέφεται 0, ενώ σε διαφορετική
περίπτωση -1.*/

int Sorted_InsertEntry( int fileDesc,
/* αναγνωριστικός αριθμός ανοίγματος αρχείου */
Record record
/* δομή που προσδιορίζει την εγγραφή */ );
/*Η συνάρτηση ​ Sorted_InsertEntry χρησιμοποιείται για την εισαγωγή μίας εγγραφής στο ταξινομημένο
αρχείο. Ο αναγνωριστικός αριθμός ανοίγματος του αρχείου δίνεται με την ​ fileDesc ενώ η εγγραφή προς
εισαγωγή προσδιορίζεται από τη δομή ​ record. Σε περίπτωση που εκτελεστεί επιτυχώς επιστρέφεται 0,
ενώ σε διαφορετική περίπτωση -1.*/

void Sorted_SortFile( char *fileName,
/* όνομα αρχείου */
int fieldNo
/* αύξων αριθμός πεδίου προς σύγκριση */);
/*Η συνάρτηση αυτή ταξινομεί ένα BF αρχείο με όνομα ​ fileName ως προς το πεδιο που προσδιοριζεται από
το ​ fieldNo. Το ​ fieldNo είναι ο αύξων αριθμός του πεδίου, δηλαδή αν το ​ fieldNo = 0, τότε το πεδίο ως προς
το οποίο θέλουμε να κάνουμε ταξινόμηση είναι το id, αν ​ fieldNo = 1, τότε το πεδίο ως προς το οποίο
θέλουμε να ταξινομήσουμε είναι το name, etc. Η συνάρτηση επιστρέφει 0 σε περίπτωση επιτυχίας ή -1 σε
διαφορετική περίπτωση.
Πιο συγκεκριμένα, η λειτουργικότητα που πρέπει να υλοποιηθεί είναι η ακόλουθη:
1) Το output file θα έχει όνομα <​ fileName>,Sorted,<​ fieldNo>. Για παράδειγμα, αν τα ορίσματα
ένουν τιμές ​ fileName = “A”, ​ fieldNo = 2, τότε το αρχείο που θα δημιουργηθεί θα έχει όνομα
“ΑSorted2”.
2) Να διαβάζονται οι εγγραφές από το αρχείο ​ fileName και να εισάγονται στο νέο αρχείο
ταξινομημένες ως προς το πεδίο με αρίθμηση ​ fieldNo. Η ταξινόμηση θα γίνει με βάση τον
αλγόριθμο εξωτερικής ταξινόμησης που έχετε διδαχθεί στο μάθημα (​ sort-merge).*/

int Sorted_checkSortedFile( char *fileName,
/* όνομα αρχείου */
int fieldNo
/* αύξων αριθμός πεδίου προς σύγκριση */ );
/*Η συνάρτηση ​ Sorted_checkSortedFile ελέγχει αν το αρχείο με όνομα ​ fileName είναι ταξινομημένο με
βάση το fieldNo πεδίο. Eπιστρέφει 0 σε περίπτωση επιτυχίας, ενώ σε διαφορετική περίπτωση -1.*/

void Sorted_GetAllEntries( int fileDesc,
/* αναγνωριστικός αριθμός ανοίγματος αρχείου */
int* fieldNo,
/* αύξων αριθμός πεδίου προς σύγκριση */
void *value
/* τιμή του πεδίου προς σύγκριση */ );
/*Η συνάρτηση αυτή χρησιμοποιείται για την εκτύπωση όλων των εγγραφών που υπάρχουν στο αρχείο
σωρού οι οποίες έχουν τιμή στο πεδίο (του οποίου ο αύξοντας αριθμός είναι ​ fieldNο) ίση με ​ value. Το
fileDesc είναι ο αναγνωριστικός αριθμός ανοίγματος του αρχείου, όπως αυτός έχει επιστραφεί από το
επίπεδο διαχείρισης μπλοκ. Η παράμετρος ​ fieldNο μπορεί να παίρνει μία από τις εξής τιμές: 0, 1, 2, 3, που
αντιστοιχούν στα πεδία μιας εγγραφής: “ id”, “​ name”, “​ surname” και “​ city”.
Για κάθε εγγραφή που βρίσκεται μέσα στο αρχείο και έχει τιμή ​ value στο πεδίο που καθορίζεται από το
fieldNο, να εκτυπώνονται τα περιεχόμενά της (συμπεριλαμβανομένου και του πεδίου-κλειδιού). Αν η
παράμετρος value είναι NULL, τότε εκτυπώνονται όλες οι εγγραφές του αρχείου. Η συνάρτηση εκτελεί
δυαδική αναζήτηση για την εύρεση των εγγραφών με τη ζητούμενη τιμή αν η παράμετρος value δεν είναι
NULL. Να εκτυπώνεται επίσης το πλήθος των μπλοκ που διαβάστηκαν.*/
#endif
