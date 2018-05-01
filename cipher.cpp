# include <errno.h>
# include <getopt.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <iostream>
# include <openssl/evp.h>
# include <openssl/rand.h>

using namespace std;

#define BUFF_SIZE (1024*1024)
#define SALT_SIZE 8

void print_usage();
void loadingBar();
void the_magician(int, int, string, int, int);

int main (int argc, char * argv[])
{
    int opt = 0; 
    char * in_path = NULL;
    char * out_path = NULL;
    int encrypt = 1;
    int fd;
    int fd2;
    int salt_set = 1;

    string pass;
  
    static struct option long_option[] ={
        {"encrypt",      no_argument,        0,        'e'},
        {"decrypt",      no_argument,        0,        'd'},
        {"nosalt",       no_argument,        0,        'n'},
        {"in",     required_argument,        0,        'i'},
        {"out",    required_argument,        0,        'o'},
        {0,                        0,        0,         0 }
    };

    int long_index = 0;
    
    while ((opt = getopt_long_only(argc, argv, "i:o:p:", long_option, &long_index))!=-1){

        switch(opt){
            case 'e': encrypt  = 1; break;
            case 'd': encrypt  = 0; break;
            case 'n': salt_set = 0; break;
            case 'i': in_path  = optarg; break;
            case 'o': out_path = optarg; break;
            case 'p': pass     = optarg; break;
            default : print_usage();
                      exit(EXIT_FAILURE);
        }
    }
    if (in_path == NULL || out_path == NULL){
        print_usage();
        exit(EXIT_FAILURE);
    }
    else{
        fd = open (in_path, O_RDWR);
        
        if ( -1 == fd){
            printf("\n open() failed with error [%s]\n",strerror(errno));
            exit(1);
        }

        fd2 = open (out_path, O_RDWR|O_CREAT, 0666);
        if (-1 == fd2){
            printf("\n open() failed with error [%s]\n",strerror(errno));
            exit(1);
        }
    }


    if (pass.empty())
    {
        string pass2;
        cout<<"Enter password: ";
        cin >> pass;
        cout <<"Verify password: ";
        cin >>pass2;
        if (pass != pass2){
            cout << "password don't match. exiting..\n";
            exit(EXIT_FAILURE);
        }
            
    }

    the_magician (fd, fd2, pass, salt_set, encrypt);

}

void the_magician (int fd, int fd2, string password, int salt_set, int enc)
{
    unsigned char   saltbuff[SALT_SIZE];
    unsigned char   *salt;
    int             readSize,
                    writeSize;

    char            header[] = "Salted__";

    if(enc == 1)
    {
    printf("\nEncrypting your file...\n");
        if (salt_set == 1)
        {
            printf("Mixing salt for better taste ;)\n");
            if(RAND_bytes(saltbuff, sizeof(saltbuff)) == 0)
                printf("failed to generate salt\n");
            write (fd2, header,sizeof(header)-1);
            write (fd2, saltbuff, SALT_SIZE);
            salt = saltbuff;
            
        }
        else{
            //nosalt
            salt = NULL;
        }
    
    }
    else 
    {    
    printf("\ndecrypting your file...\n");
        if (salt_set == 1)
        {
            lseek(fd, sizeof(header)-1, SEEK_SET);

            if(read(fd, saltbuff,SALT_SIZE) == -1)
                printf("failed to read salt from file\n");
            salt = saltbuff;

        }
        else {
            salt = NULL;
        }
    }
    loadingBar();

    //Setting up the cipher type
    const EVP_CIPHER * cipher = EVP_rc4();
    //digest to use later
    const EVP_MD * dgst = EVP_md5();

    unsigned char key[EVP_MAX_KEY_LENGTH], iv[EVP_MAX_IV_LENGTH];

    //taking care of password
    char newPass [password.length()]; 
    strcpy (newPass, password.c_str());

    //derives a key from a password using a salt and iteration count
    EVP_BytesToKey(cipher, dgst, salt,
                           (unsigned char *)newPass, strlen(newPass), 1, key, iv);

    //Creates a cipher context
    EVP_CIPHER_CTX ctx;

    //initializes cipher contex ctx.
    EVP_CIPHER_CTX_init(&ctx);

    //set key and IV and other params
    EVP_CipherInit_ex(&ctx, EVP_rc4(), NULL, key, iv, enc);

    unsigned char buf_in[BUFF_SIZE], buf_out[BUFF_SIZE + EVP_MAX_BLOCK_LENGTH];
    

    readSize = read(fd, buf_in, BUFF_SIZE);
        
    if(readSize <= 0)
        cout<<"readSize <= 0\n";  //debug
            // break;
        
    if(EVP_CipherUpdate(&ctx, buf_out, &writeSize, buf_in, readSize) == 0)
    {
        EVP_CIPHER_CTX_cleanup(&ctx);
        printf("Error: EVP_CipherUpdate");
    }
        
    write (fd2, buf_out, writeSize);

    if(EVP_CipherFinal_ex(&ctx, buf_out, &writeSize) == 0)
    {
        EVP_CIPHER_CTX_cleanup(&ctx);
        printf("Error: EVP_CipherFInal_ex");
    }
    write(fd2, buf_out, writeSize);

    EVP_CIPHER_CTX_cleanup(&ctx);



    exit(EXIT_SUCCESS);

}

void print_usage(){
    printf("Usage: ./rc4 -[e|d] [-nosalt] -in infile -out outfile -p password\n");
}
void loadingBar(){
    cout << endl;
    for (int i=0; i <= 100; ++i)
    {
        string progress = "[" + string(i, '*') + string(100-i, ' ') + "]";
        cout << "\r\033[F" << "\n" << progress << flush;
        usleep(10000);
        cout <<"  "<< i << "%";
    }
    cout<<" success!\n";
}