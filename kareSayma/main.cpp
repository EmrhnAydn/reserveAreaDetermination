#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphics.h>
#include <math.h>
#include <curl/curl.h>

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream); // curl 
void eksenciz(); // 2 boyutlu uzay duzlemini cizdiriyoruz
char* parcala(const char* str); //alinan verinin kullanilacak kismini belirliyor  
int* sayilastir(char *a); //stringi integera ceviriyor
int sekilsayisi(int* sayilar, int boyut,int *ay); // veride kac sekil oldugunu belirliyor
void alanVeKareSayisiYazdir(int* coordinates, int size, float *topl,int* bks); // Alan ve Plartform degerlerini buluyor
const char* satir(const char* text, int line_number); // gelen veride secilen satiri donduruyor
bool icindemi(int* polygon, int n, int x, int y); // birim karenin icersinde poligon bulunup bulunmadigini kontrol ediyor
void platformbelirleme(int* polygon, int n); // plartformun oldugu kareleri griye boyuyor
int platformSayisi(int* polygon, int n); // plartform sayisini donduruyor

int main()
{
	//Curl islemleri
	FILE *dosya;
    CURL *curl;
    CURLcode res;
    long file_size;
    int secim;
	float pMal; //plartform maliyeti
	int sMal; // sondaj maliyeti
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://abilgisayar.kocaeli.edu.tr/prolab1/prolab1.txt");

        dosya = fopen("yaz1.txt", "wb");
        if (!dosya) {
            fprintf(stderr, "Dosya açma hatasý\n");
            return 1;
        }

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, dosya);

        res = curl_easy_perform(curl);
        fclose(dosya);

        if (res != CURLE_OK) {
            fprintf(stderr, "cURL hatasý: %s\n", curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            return 1;
        }
		dosya = fopen("yaz1.txt", "r");
        fseek(dosya, 0, SEEK_END);
        file_size = ftell(dosya);
        rewind(dosya);

        char metin[file_size + 1];
        fread(metin, 1, file_size, dosya);
        metin[file_size] = '\0'; 
		printf("Cekilen veri:\n%s\n\n",metin);
		
        printf("Hangi satiri cizdirmek istersiniz:");
        scanf("%d", &secim);
        
        printf("\nBirim Sondaj Maliyetini Giriniz (1-10):");
        scanf("%d",&sMal);
        
        printf("\nBirim Plartform Maliyetini Giriniz:");
        scanf("%f",&pMal);
        
        const char* input_str = satir(metin, secim); // verideki secilen satiri input_str ye esitliyoruz 
		printf("\nSecilen satir: %s\n", input_str);
	
	// input islemleri
	char* dnm = parcala(input_str); // kullanilacak veriyi dnm ye esitle 
	int* coord = sayilastir(dnm); // koordinat noktalarini integer sekilde coorda esitliyoruz
	
	int boyut= 0;
	for(int i=0;coord[i] != '\0';i++)
	{
		boyut++; // boyut bilgisi
	}
	for(int i=0;coord[i] != '\0';i++)
	{
		coord[i] = coord[i] *16; // cizdirdiðimiz eksenin boyutlarýna uygun pikseldeki noktayi bulmak için 16 ile carpiyoruz 
	}
	initwindow(1920,1080); // pencere ac
	int ayrim; // iki sekil varsa ayrim indeksnin temsil eder
	eksenciz();
	if(sekilsayisi(coord,boyut,&ayrim))
	{ 
		//Tek Sekil varsa
	float toplmal=0;
	int birimks;
 	setfillstyle(1, RED);
    fillpoly(boyut/2, coord);
    alanVeKareSayisiYazdir(coord, boyut, &toplmal,&birimks);
    platformbelirleme(coord, boyut);
    
    printf("\nToplam Plartform Sayisi:%d\n",birimks);
    
    printf("\nToplam Sondaj Maliyeti: %.2f\n", (float(sMal) * toplmal /10.0));
    
	printf("\nToplam Plartform Maliyeti: %.2f\n", (pMal * float(birimks)));
	
	printf("\nToplam Maliyet: %.2f\n",((float(sMal) * toplmal /10.0) + (pMal * float(birimks))));
	
	printf("\nKar Miktari: %.2f\n", toplmal - ((float(sMal) * toplmal /10.0) + (pMal * float(birimks))));  	
		
	setfillstyle(1, RED);
    fillpoly(boyut/2, coord);
     	char floatStr[20];
   	 	sprintf(floatStr, "%.2f", toplmal);
        
        outtextxy(500, 20, "Toplam Rezerv Deger:");
   		outtextxy(680, 20, floatStr);
	}
	else
	{
		// cift sekil varsa
		int* sekil1 = (int*)malloc((ayrim+2)* sizeof(int)); // sekil1 in koordinatlari icim bellekte alan tuttuk
		int* sekil2 = (int*)malloc( (boyut - ayrim)* sizeof(int)); //sekil2 nin koordinatlari icin bellekte alan tuttuk
		float toplam_alan = 0.0;
		float tAlan= 0.0;
		int i=0;
		int boyut1=0;
		int boyut2=0;
		int tPLart = 0; //toplam plartform sayisini verecek
		int toplam_plartf;
		for(i=0;i <= ayrim;i++)
		{
			sekil1[i] = coord[i];
			boyut1++;
		}
		sekil1[i+1] = '\0';
		for(;coord[i] != '\0';i++)
		{
			sekil2[boyut2] = coord[i];
			boyut2++;
		}
		sekil2[boyut2+1] = '\0';
		setfillstyle(1, RED);
        fillpoly(boyut1/ 2, sekil1);
        fillpoly(boyut2/ 2, sekil2);
        //ilk sekilin kareleri
        
        alanVeKareSayisiYazdir(sekil1, boyut1,&toplam_alan,&toplam_plartf);
        tAlan += toplam_alan;
        tPLart += toplam_plartf;
        //toplam_alan =0;

        
    	platformbelirleme(sekil1, boyut1);
    	platformbelirleme(sekil2, boyut2); // Kullanýcý bir tuþa bastýktan sonra kareler boyanýr.
    	
    	setfillstyle(1, RED);
        fillpoly(boyut1/ 2, sekil1);
        fillpoly(boyut2/ 2, sekil2);
        
        //ikinci seklin kareleri
   	 	alanVeKareSayisiYazdir(sekil2, boyut2,&toplam_alan,&toplam_plartf);
   	 	tAlan += toplam_alan;
   	 	tPLart += toplam_plartf;
		
   	 	printf("\nToplam Sondaj Maliyeti: %.2f", (float(sMal) * tAlan /10.0));
   	 
   	 	
    	setfillstyle(1, RED);
        fillpoly(boyut2/ 2, sekil2);
        
        printf("\nToplam Plartform Sayisi:%d",tPLart);
        
        printf("\nToplam Plartform Maliyeti: %.2f\n", (pMal * float(tPLart)));
        
        printf("\nToplam Maliyet: %.2f\n",((float(sMal) * tAlan /10.0) + (pMal * float(tPLart))));
        
        printf("\nKar Miktari: %.2f\n", tAlan - ((float(sMal) * tAlan /10.0) + (pMal * float(tPLart)))); 
        
        char floatString[20];
   	 	sprintf(floatString, "%.2f", tAlan);
        
        outtextxy(500, 20, "Toplam Rezerv Deger:");
   		outtextxy(680, 20, floatString);
   		
	}
	
	getch();
    closegraph();
    //dosya islemlerini bitir
	fclose(dosya);
    curl_easy_cleanup(curl); 
	}
	else
	{
		fprintf(stderr, "cURL baþlatma hatasi\n");
        return 1;
	}
 	return 0;
}
void eksenciz()
{
	 for (int i = 0; i < 1081; i += 16) 
	 {
        for (int j = 0; j < 1921; j += 16) 
		{
			setcolor(GREEN);
            rectangle(j, i, j + 16, i + 16);
            
        }
        Sleep(25);
	}
}



char* parcala(const char* str)
{
	
	int i, basla, bitir;
	for(i=0;;i++)
	{
		if(str[i] == 'B')
		{
			basla = i+2;
		}
		else if(str[i]== 'F')
		{
			bitir = i-2;
			break;
		}
	}
	
	int coorSz = bitir - basla + 4; // Null karakter için bir ")(" için 2 fazladan memmory size ayir
	char* coor = (char*)malloc(( coorSz * sizeof(char))); // )( bu ifade disindaki kisimlari parcala
	int j=0;
	for(i = basla; i <= bitir;i++)
	{
			coor[j]= str[i];
			j++;
	}
	coor[j] = ')';
	coor[j+1] = '('; // )( token icin ayirici karkaterler
	coor[j+2] = '\0';
	
 	return coor;	
}


int* sayilastir(char *a)
{
	char* temp = a; // yedekledik
	int sizeC = 0; 
	for(int i=0; a[i] != '\0';i++)
	{
		if(a[i] == ')')
		{
			sizeC++; 
		}
	}
	sizeC = sizeC*2 +1; // noktalarimizi olusturan toplam x ve y sayisini buluyoruz en sona +1 Null karkater
	int* coordinates = (int*)malloc((sizeC * sizeof(int)));
	char *token = strtok(a, ")(");
	int index = 0;
	while (token != NULL) {
        int x, y;
        sscanf(token, "%d,%d", &x, &y); // char i integera ceviren parca
        coordinates[index++] = x;
        coordinates[index++] = y;
		token = strtok(NULL, ")(");
    }
    coordinates[index] = '\0'; 
	return coordinates;
}

int sekilsayisi(int* sayilar, int boyut, int *ay)
{
	int i,j;
	int k=0;
	int ayrim; // eger iki sekil varsa ayrilacak noktayý temsil eder
	for(i=3;i<boyut;i+=2)
	{
		for(int j=0;j<i-1;j+=2)	
		{
			if(sayilar[i] ==  sayilar[j+1] && sayilar[i-1] ==  sayilar[j])
			{
				k++;
				if (k == 1) 
				{
       			*ay = i;
    			} 
    			else if (k > 1) 
				{
       			return 0; // iki sekil var
    			}
			}
    		
		}
	}
	return 1;
}

void alanVeKareSayisiYazdir(int* coordinates, int size,float* topl ,int* bks) {
    double alan = 0.0;
    int j = size - 2;
    float deger;
	*topl =0;	
    for (int i = 0; i < size; i += 2) {
        alan += (coordinates[j] + coordinates[i]) * (coordinates[j + 1] - coordinates[i + 1]);
        j = i;
    }
	
    alan = fabs(alan/256.0) / 2.0;
    deger = alan*10.0;
    *topl = deger;
    char areaStr[50];
    sprintf(areaStr, "Rezerv Alan: %.2f, Rezerv Deger: %.2f", alan, deger);
    // En düþük y koordinatýný (en üst nokta) ve karþýlýk gelen x koordinatýný bul
    int minY = coordinates[1];
    int minX = coordinates[0];

    for (int i = 3; i < size; i += 2) {
        if (coordinates[i] < minY) {
            minY = coordinates[i];
            minX = coordinates[i-1];
        }
    }

    // En üst noktayý kullanarak metni çiz
    setcolor(WHITE); // Metin rengini ayarla
    outtextxy(minX, minY -40 , areaStr); // minY'den 40 piksel yukarýda metni yerleþtir
    getch();

    int squareCount = platformSayisi(coordinates, size);
    *bks = squareCount;
    char squareCountStr[50];
    sprintf(squareCountStr, "Plartfrom sayisi: %d", squareCount);
    outtextxy(minX , minY-20 , squareCountStr); // alan metninin hemen altýna yaz
}


size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream){
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

//curl fonksiyonlari
const char* satir(const char* text, int line_number) {
    static char buffer[1000];
    const char *start = text;
    int current_line = 1;

    while (*start != '\0' && current_line < line_number) {
        if (*start == '\n') {
            current_line++;
        }
        start++;
    }

    if (*start == '\0') {
        return "Bu numarada satir bulunamadý.\n";
    }

    const char *end = strchr(start, '\n');
    int length = end ? (end - start) : (strlen(start));
    strncpy(buffer, start, length);
    buffer[length] = '\0';
    return buffer;
}

// plartform fonks

int getColorAt(int x, int y) {
    return getpixel(x, y);
}

int platformSayisi(int* polygon, int n) {
    int count = 0;
    for (int y = 0; y < 1080; y += 16) {
        for (int x = 0; x < 1920; x += 16) {
            bool top_left = icindemi(polygon, n / 2, x, y);
            bool top_right = icindemi(polygon, n / 2, x + 16, y);
            bool bottom_left = icindemi(polygon, n / 2, x, y + 16);
            bool bottom_right = icindemi(polygon, n / 2, x + 16, y + 16);

            // Check if any corner of the square is inside the polygon
            if (top_left || top_right || bottom_left || bottom_right) {
                // Check if there is red color inside the square
                bool hasRedColor =
                    (getColorAt(x, y) == RED) ||
                    (getColorAt(x + 16, y) == RED) ||
                    (getColorAt(x, y + 16) == RED) ||
                    (getColorAt(x + 16, y + 16) == RED);

                if (hasRedColor) {
                    count++;
                }
            }
        }
    }
    return count;
}

bool icindemi(int* polygon, int n, int x, int y) {
    int count = 0, i, next;
    for (i = 0; i < n; i++) {
        next = (i + 1) % n;
        if ((y > polygon[i * 2 + 1] && y <= polygon[next * 2 + 1]) || (y > polygon[next * 2 + 1] && y <= polygon[i * 2 + 1])) {
            if (x <= (polygon[next * 2] - polygon[i * 2]) * (y - polygon[i * 2 + 1]) / (polygon[next * 2 + 1] - polygon[i * 2 + 1]) + polygon[i * 2])
                count++;
        }
    }
    return count % 2;
}

void platformbelirleme(int* polygon, int n) {
    for (int y = 0; y < 1080; y += 16) {
        for (int x = 0; x < 1920; x += 16) {
            bool hasRedColor = false;

            for (int i = 0; i < 16; ++i) {
                for (int j = 0; j < 16; ++j) {
                    if (getColorAt(x + i, y + j) == RED) {
                        hasRedColor = true;
                        break;
                    }
                }
                if (hasRedColor) {
                    break;
                }
            }

            if (hasRedColor) {
                setfillstyle(SOLID_FILL, LIGHTGRAY);
                bar(x, y, x + 16, y + 16);
            }
        }
    }
}

