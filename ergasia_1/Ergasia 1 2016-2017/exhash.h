typedef struct Record {
	int id;
	char name[15];
	char surname[20];
	char city[25];
} Record;


/* H δομή όπου κρατούνται όσες πληροφορίες κρίνετε απαραίτητες προκειμένου να μπορείτε να 
επεξεργαστείτε στη συνέχεια τις εγγραφές του. Tα attrName, attrType και attrLength αφορούν το 
πεδίο-κλειδί, fileDesc είναι ο αναγνωριστικός αριθμός του ανοίγματος του αρχείου, όπως 
επιστράφηκε από το επίπεδο διαχείρισης μπλοκ και depth είναι το ολικό βάθος του ευρετηρίου 
επεκτατού κατακερματισμού. */
typedef struct {
    int fileDesc;          /* αναγνωριστικός αριθμός ανοίγματος αρχείου από το επίπεδο block */ 
    char* attrName;    /* το όνομα του πεδίου που είναι το κλειδί για το συγκεκριμένο αρχείο */ 
    char attrType;                                        /* τύπος πεδίου-κλειδιού: 'c', 'i' */ 
    int attrLength;                                                 /* μήκος πεδίου-κλειδιού */ 
    int depth;                         /* το ολικό βάθος ευρετηρίου επεκτατού κατακερματισμού */ 
} EH_info;

/* arxikopoei to bucket.To topiko vathos ginetai iso me depth kai o ari8mos twn eggrafwn 0*/
void init_ET_bucket(int blockNumber,int fileDesc,int depth);

/*epistrefei to size tou evrethriou*/
int IndexSize(int depth);

/* metatrepei enan dekadiko akeraio se binary kai ton epistrefei se morfh symvoloseiras*/
char * decimalToBinary(int n);


/*metatrepei enan binary se dekadiko kai ton epistrefei*/
int BinaryToDecimal(char * binary);

/* kanei hashing kai epistrefei to apotelesma tou*/
int hashFunction(int n,int depth);


/* diplasiazei to mege8os tou evrethriou*/
void doubleIndex(EH_info* header_info,int numberOfChangedBlock);

/*desmevei buckets stis 8eseis tou evrethriou pou prin edeixnan se kapoio allo gemato bucket*/
void allocateBuckets(EH_info* header_info,int numberOfChangedBlock);

/*grafe mia egrgafh se ena bucket an xwraei.Diaforetika kanei tis katallhles energeies wste na dhmiourgh8ei xwros gia thn eisagwgh ths eggrafhs*/
void WriteToBucket(Record rec,int numberOfBlock,EH_info* header_info);

char * ET_findTypeOfFile(int fd);


/* Η συνάρτηση EH_CreateIndex χρησιμοποιείται για τη δημιουργία και την κατάλληλη αρχικοποίηση 
ενός άδειου αρχείου επεκτατού κατακερματισμού με όνομα fileName. Σε περίπτωση επιτυχίας 
επιστρέφει 0, ενώ σε περίπτωση αποτυχίας επιστρέφει -1. */
int EH_CreateIndex(
    char *fileName,                                                         /* όνομα αρχείου */ 
    char* attrName,    /* το όνομα του πεδίου που είναι το κλειδί για το συγκεκριμένο αρχείο */ 
    char attrType,                                        /* τύπος πεδίου-κλειδιού: 'c', 'i' */ 
    int attrLength,                                                 /* μήκος πεδίου-κλειδιού */ 
    int depth                         /* το ολικό βάθος ευρετηρίου επεκτατού κατακερματισμού */ 
);



/* Η συνάρτηση ΕH_OpenIndex ανοίγει το αρχείο με όνομα filename και διαβάζει την μετα-πληροφορία 
του αρχείου επεκτατού κατακερματισμού. Κατόπιν, ενημερώνει μια δομή όπου κρατούνται όσες 
πληροφορίες κρίνετε απαραίτητες προκειμένου να μπορείτε να επεξεργαστείτε στη συνέχεια τις 
εγγραφές του. Σε περίπτωση επιτυχίας επιστρέφει τη δομή πληροφοριών του αρχείου, ενώ σε περίπτωση
αποτυχίας επιστρέφει τιμή NULL. Αν το αρχείο που δοθηκε για άνοιγμα δεν αφορά αρχείο επεκτατού 
κατακερματισμού, τότε αυτό επίσης θεωρείται σφάλμα.*/
EH_info* EH_OpenIndex( char *fileName );                       /* όνομα αρχείου προς άνοιγμα */


/* Η συνάρτηση ΕH_CloseIndex κλείνει το αρχείο επεκτατού κατακερματισμού που προσδιορίζεται με 
τη δομή header_info. Η συνάρτηση είναι υπεύθυνη και για την αποδέσμευση της μνήμης που καταλαμβάνει 
η δομή που περάστηκε ως παράμετρος, στην περίπτωση που το κλείσιμο πραγματοποιήθηκε επιτυχώς. Σε 
περίπτωση επιτυχίας επιστρέφει 0, ενώ σε διαφορετική περίπτωση -1.  */
int EH_CloseIndex( EH_info* header_info );         /* πληροφορίες του αρχείου προς κλείσιμο */



/* Η συνάρτηση ΕH_InsertEntry εισάγει μία εγγραφή στο αρχείο επεκτατού κατακερματισμού, σύμφωνα 
και με την αντίστοιχη θεωρία. Οι πληροφορίες που αφορούν στο αρχείο βρίσκονται στη δομή header_info, 
όπως αυτές επιστράφηκαν από την ΕH_OpenIndex, ενώ η εγγραφή προς εισαγωγή δίνεται στην παράμετρο 
record. Σε περίπτωση που εκτελεστεί επιτυχώς, επιστρέφει 0, ενώ σε διαφορετική περίπτωση -1.  */
int EH_InsertEntry(
    EH_info* header_info,                                         /* επικεφαλίδα του αρχείου*/
    Record record                                      /* δομή που προσδιορίζει την εγγραφή */
);



/* Η συνάρτηση αυτή χρησιμοποιείται για την εκτύπωση όλων των εγγραφών που υπάρχουν στο αρχείο 
επεκτατού κατακερματισμού οι οποίες έχουν τιμή στο πεδίο-κλειδί ίση με value. Η παράμετρος 
header_info δίνει πληροφορίες για το αρχείο κατακερματισμού, όπως είχαν επιστραφεί από την 
ΕH_OpenIndex. Για κάθε εγγραφή του αρχείου με τιμή στο πεδίο-κλειδί ίση με value, εκτυπώνονται 
τα περιεχόμενά της (συμπεριλαμβανομένου και του πεδίου-κλειδιού). Να εκτυπώνει επίσης το πλήθος 
των blocks που διαβάστηκαν μέχρι να βρεθούν όλες οι εγγραφές. Σε περίπτωση επιτυχίας επιστρέφει 
το πλήθος των εγγραφών που τυπώθηκαν, ενώ σε περίπτωση λάθους επιστρέφει -1.  */
int EH_GetAllEntries(
    EH_info header_info,                                         /* επικεφαλίδα του αρχείου */ 
    void *value                                  /* τιμή του πεδίου-κλειδιού προς αναζήτηση */
); 



/* Η συνάρτηση διαβάζει το αρχείο με όνομα filename και τυπώνει τα στατιστικά που αναφέρθηκαν 
προηγουμένως. Σε περίπτωση επιτυχίας επιστρέφει 0, ενώ σε περίπτωση λάθους επιστρέφει -1. */
int HashStatistics( char* filename );                   /* όνομα του αρχείου που ενδιαφέρει */

