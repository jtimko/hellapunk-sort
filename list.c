#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void makeCleanList();
void parseNewList();

int main() {

    makeCleanList();
    parseNewList();
    
    return 0;
}

void makeCleanList() {
    FILE *fp;
    FILE *fw;
    fp = fopen("thelist.txt", "r");
    fw = fopen("newlist.txt", "a");

    int c, d;

    while ((c = fgetc(fp)) != EOF) {

        /******************************************************
            space    = 32
            new line = 10
            asterik  = 42
            pound    = 35
            at sign  = 64
        *******************************************************/

        d = fgetc(fp);

        /**************************************************************
         *  This is reformatting some of Steves list. It is removing
         *  double spaces, lines that have a \n and space (seen with
         *  steves listing of the venues), and removing *,#,@ which
         * he uses as a keys
         * ***********************************************************/
        if (((c == 32) && (d == 32)) || (c == 10 && d == 32) || c == 42 || c == 35 || c == 64)
            printf("");
        else
            fputc(c, fw);
        
        // Resetting the character
        c = ungetc(d, fp);
    }
    fclose(fw);
    fclose(fp);

}

void parseNewList() {
    FILE *fp, *fw;
    fp = fopen("newlist.txt", "r");
    char buff[255], bands[500], month[10], wrdday[10], date[20], venue[40], showtime[15];
    char needle[] = "pm";
    char *p;
    int day, i, j, k;
    int count = 0;

    while (fgets(buff, sizeof(buff), fp)) {
        if ((p = strstr(buff, needle)) != NULL) {
            fw = fopen("hellapunk.txt", "a"); 
        
        //Character counter for each line.    
        for (i = 0; buff[i] != '\0'; i++)
            ;

        /*
            Trying to clean up
        */
        k = i; //Find a better way;

        while ((buff[k] != 109) && (buff[k - 1] != 112)) {
            buff[k] = '\0';
            k--;
        }

        /* 
            separates the first three words which happens
            to be the dates 
        */
        sscanf(buff, "%s %d %s", month, &day, wrdday);
        snprintf(date, sizeof(date), "%s %d %s", month, day, wrdday);

        // Character counter for remaining of the string 
    
        for (j = 0; date[j] != '\0'; ++j)
            ;

        /*
            Separating the list of bands from the buffer.
            The if statement is looking for " at " which is 
            usually followed by venue, city, price.
        */
        j++;
        for (j; j <= i; j++) {

            if ((buff[j-1] == 32) && (buff[j] == 97) && (buff[j+1] == 116) && (buff[j+2] == 32))
                break;
            else {
                bands[count] = buff[j];
                count++;
            }
        }

        /*
            Found " at ". Use j+3 to skip the at and space.
            The next part of the string is the venue. The 
            venue ends with a comma, so copy until then.
        */
        j = j+3;

        count = 0;
        for (j; j<=i; j++) {
            if (buff[j] == 44)
                break;
            else {
                venue[count] = buff[j];
                count++;
            }
        }


        /*
            For the time we go to the end of the string and count
            backwards until we reach a space. Usually the time
            is the last part of the line.
        */

        count = 0;

        for (i = 0; buff[i] != '\0'; i++)
            ;

        for (--i; i > 0; --i) {
            if (buff[i] == 32 ) {
                i++;
                break;
            }
        }
        while (buff[i] != '\0') {
            showtime[count] = buff[i];
            count++;
            i++;
        }
        fprintf(fw, "%s;%s;%s;%s\n", bands, date, venue, showtime);
        fclose(fw);



        /******************************************************
         *              RESET EVERYTHING                      *
         *****************************************************/
        count = 0;
        memset(&buff, 0, sizeof(buff));
        memset(&bands, 0, sizeof(bands));
        memset(&month, 0, sizeof(month));
        memset(&wrdday, 0, sizeof(wrdday));
        memset(&date, 0, sizeof(date));
        memset(&venue, 0, sizeof(venue));
        memset(&showtime, 0, sizeof(showtime));
    }
        }

    fclose(fp);
}