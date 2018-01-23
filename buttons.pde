void enableMenus(int start , int end)
{
    for(int i = start ; i <= end ; i++)
        menuDisabled[i] = false;
}

void disableMenus()
{
    for(int i = 0 ; i < menuDisabled.length ; i++)
        menuDisabled[i] = true;
}

//*****************************************Création de bouttons*********************************************
void newMenu(int x,int y,String text,int couleur,int tailleX,int tailleY,boolean isDisabled)
{
    try {
        numberOfMenu = numberOfMenu + 1;
        menuDisabled[numberOfMenu-1] = isDisabled;
        menuPos[2*numberOfMenu-2] = x;
        menuPos[2*numberOfMenu-1] = y;
        menuContent[numberOfMenu-1] = text;
        menuColor[numberOfMenu-1] = couleur;
        menuSize[2*numberOfMenu-2] = tailleX;
        menuSize[2*numberOfMenu-1] = tailleY;
    } catch(ArrayIndexOutOfBoundsException e) {
        errorMsg("",SFX[1],e);
    } catch(Exception e) {
        errorMsg("",SFX[1],e);
    }
}

//*****************************************Suppression des bouttons de menu*********************************************
void delMenus()
{
    menuPos = new int[200];
    menuColor = new int[100];
    menuSize = new int[200];
    menuContent = new String[100];
    menuDisabled = new boolean[100];
    numberOfMenu = 0;
}

void drawMenus()
{
    if(menu < 6 && menu != 3)
        enableMenus(23,23);
    if(nbOfAchievementsLoaded > 0)
        try {
            achievementButtonImage = trophys[floor(3*nbOfAchievementsGot/nbOfAchievementsLoaded)];
        } catch(Exception e) {
            achievementButtonImage = redCross;
        }
    else
        achievementButtonImage = trophys[0];
    if(menu == -1) {
        menuPos[46] = width-25;
        menuPos[47] = 100;
    } else if(menu >= 0 && menu <= 6) {
        tint(255,150);
        menuPos[46] = width-25;
        menuPos[47] = 20;
    }
    for (int i = 0; i < numberOfMenu; i++) {
        if (!menuDisabled[i]) {
            noStroke();
            int current_color = 0;
            int alph = 255;
            if (mouseX <= menuPos[2*i]+menuSize[2*i] && mouseX >= menuPos[2*i] && mouseY <= menuPos[2*i+1]+menuSize[2*i+1] && mouseY >= menuPos[2*i+1]) {
                current_color = menuColor[i]-100;
                if (i == 1 && compareStrings(language, "yolo"))
                    bug = godEater;
                else if (i == 1)
                    bug = bugRed;
                if (menu == 7) {
                    current_color = color(menuColor[i]);
                    alph = 10;
                }
            } else {
                if (i == 1)
                    bug = bugImage;
                if (menu == 0) {
                    current_color = color(menuColor[i]);
                    alph = 10;
                } else
                    current_color = menuColor[i];
            }
            tint(255, 255, 255, alph+20);
            fill(current_color-150, alph);
            rect(menuPos[2*i], menuPos[2*i+1], menuSize[2*i], menuSize[2*i+1]);
            fill(current_color-125, alph);
            rect(menuPos[2*i]+1, menuPos[2*i+1]+1, menuSize[2*i]-2, menuSize[2*i+1]-2);
            fill(current_color-100, alph);
            rect(menuPos[2*i]+2, menuPos[2*i+1]+2, menuSize[2*i]-4, menuSize[2*i+1]-4);
            fill(current_color-80, alph);
            rect(menuPos[2*i]+3, menuPos[2*i+1]+3, menuSize[2*i]-6, menuSize[2*i+1]-6);
            fill(current_color-60, alph);
            rect(menuPos[2*i]+4, menuPos[2*i+1]+4, menuSize[2*i]-8, menuSize[2*i+1]-8);
            fill(current_color-40, alph);
            rect(menuPos[2*i]+5, menuPos[2*i+1]+5, menuSize[2*i]-10, menuSize[2*i+1]-10);
            fill(current_color-25, alph);
            rect(menuPos[2*i]+6, menuPos[2*i+1]+6, menuSize[2*i]-12, menuSize[2*i+1]-12);
            fill(current_color-10, alph);
            rect(menuPos[2*i]+7, menuPos[2*i+1]+7, menuSize[2*i]-14, menuSize[2*i+1]-14);
            fill(current_color, alph);
            rect(menuPos[2*i]+8, menuPos[2*i+1]+8, menuSize[2*i]-16, menuSize[2*i+1]-16);
            fill(0);
            if (mouseX <= menuPos[2*i]+menuSize[2*i] && mouseX >= menuPos[2*i] && mouseY <= menuPos[2*i+1]+menuSize[2*i+1] && mouseY >= menuPos[2*i+1] && i == 16)
                fill(0, 150);
            if (i == 1)
                try {
                    image(bug, menuPos[2*i], menuPos[2*i+1]);
                }
                catch(Exception e) {
                    e.printStackTrace();
                    error("Error 10",e);
                }
            else if (i == 4) {
                try {
                    image(youtube, menuPos[2*i]+1, menuPos[2*i+1]+1);
                }
                catch(Exception e) {
                    e.printStackTrace();
                    error("Error 11",e);
                }
            } else if(i == 23) {
                try {
                    image(achievementButtonImage, menuPos[2*i], menuPos[2*i+1]);
                }
                catch(Exception e) {
                    try {
                      image(glitched_texture, menuPos[2*i], menuPos[2*i+1]);
                    }
                    catch(Exception f) {
                      e.printStackTrace();
                      f.printStackTrace();
                      errorMsg("Error 10 "+e+"\n"+f,SFX[1],e);
                    }
                }
            }
            textSize(30);
            if (compareStrings(language, "yolo"))
                textSize(random(1, 50));
            text(menuContent[i], menuPos[2*i]+4, menuPos[2*i+1]+menuSize[2*i+1]-10);
            tint(255);
        }
    }
}

//Créations des bouttons
void classicButtons()
{
    if(compareStrings("fr",language)) {
        newMenu(10,10,"Jouer",255,140,40,true);                          //0
        newMenu(width-32,height-32,"",255,32,32,true);                   //1
        newMenu(10,60,"Paramètres",255,170,40,true);                     //2
        newMenu(10,110,"Quitter",255,140,40,true);                       //3
        newMenu(width-65,height-32,"",255,33,32,true);                   //4
        newMenu(160,10,"Options",255,140,40,true);                       //5
        newMenu(190,60,"Audio",255,110,40,true);                         //6
        newMenu(160,210,"Retour",255,140,40,true);                       //7
        newMenu(160,110,"Langue",255,140,40,true);                       //8
        newMenu(width/2-70,10,"Français",255,140,40,true);               //9
        newMenu(width/2-70,60,"English",255,140,40,true);                //10
        newMenu(width-150,height-50,"Annuler",255,140,40,true);          //11
        newMenu(10,height-50,"Sauvegarder",255,180,40,true);             //12
        newMenu(160,160,"Controles",255,140,40,true);                    //13
        newMenu(100,10,"",255,140,40,true);                              //14
        newMenu(100,60,"",255,140,40,true);                              //15
        newMenu(width-180,height-40,"Sauvegarder",255,180,40,true);      //16
        newMenu(width/2-70,110,"Deutsch",255,140,40,true);               //17
        newMenu(width/2-100,height/2-40,"Yes",255,80,80,true);           //18
        newMenu(width/2+20,height/2-40,"No",255,80,80,true);             //19
        newMenu(0,height - 40,"Ajouter un personnage",255,300,40,true);  //20
        newMenu(0,height - 80,"Menu des objets",255,240,40,true);        //21
        newMenu(width-150,height-50,"Retour",255,140,40,true);           //22
        newMenu(width-24,50,"",255,25,25,true); //achievements           //23
        newMenu(width-150,height-50,"Retour",255,140,40,true);           //24
        newMenu(200,10,"",color(25,25,255),80,40,true);                  //25
      
    } else if(compareStrings("en",language)) {
        newMenu(10,10,"Play",255,140,40,true);
        newMenu(width-32,height-32,"",255,32,32,true); //rapport de bug
        newMenu(10,60,"Settings",255,140,40,true);
        newMenu(10,110,"Quit",255,140,40,true);
        newMenu(width-65,height-32,"",255,33,32,true); //youtube
        newMenu(160,10,"Options",255,140,40,true);
        newMenu(160,60,"Sound",255,140,40,true);
        newMenu(160,210,"Back",255,140,40,true);
        newMenu(160,110,"Language",255,140,40,true);
        newMenu(width/2-70,10,"Français",255,140,40,true);
        newMenu(width/2-70,60,"English",255,140,40,true);
        newMenu(width-150,height-50,"Cancel",255,140,40,true);
        newMenu(10,height-50,"Save",255,100,40,true);
        newMenu(160,160,"Controls",255,140,40,true);
        newMenu(100,10,"",255,140,40,true); //on/off (music volume)
        newMenu(100,60,"",255,140,40,true); //% music volume
        newMenu(width-180,height-40,"Save",255,180,40,true);
        newMenu(width/2-70,110,"Deutsch",255,140,40,true);
        newMenu(width/2-100,height/2-40,"Yes",255,80,80,true);
        newMenu(width/2+20,height/2-40,"No",255,80,80,true);
        newMenu(0,height-40,"Add a character",255,240,40,true);
        newMenu(0,height-80,"Objetcts select",255,240,40,true);
        newMenu(width-150,height-50,"Back",255,140,40,true);
        newMenu(width-24,50,"",255,25,25,true); //achievements
        newMenu(width-150,height-50,"Back",255,140,40,true);
        newMenu(200,10,"",color(25,25,255),80,40,true);                //25
      
    } else if(compareStrings("de",language)) {
        newMenu(10,10,"Spielen",255,140,40,true);
        newMenu(width-32,height-32,"",255,32,32,true);
        newMenu(10,60,"Einstellungen",255,181,40,true);
        newMenu(10,110,"Verlassen",255,140,40,true);
        newMenu(width-65,height-32,"",255,33,32,true);
        newMenu(160,10,"Optionen",255,140,40,true);
        newMenu(201,60,"Klingen",255,99,40,true);
        newMenu(160,210,"Zurück",255,140,40,true);
        newMenu(160,110,"Sprache",255,140,40,true);
        newMenu(width/2-70,10,"Français",255,140,40,true);
        newMenu(width/2-70,60,"English",255,140,40,true);
        newMenu(width-150,height-50,"Stornieren",255,140,40,true);
        newMenu(10,height-50,"Speichern",255,140,40,true);
        newMenu(160,160,"Steuert",255,140,40,true);
        newMenu(100,10,"",255,140,40,true);
        newMenu(100,60,"",255,140,40,true);
        newMenu(width-180,height-40,"Speichern",255,180,40,true);
        newMenu(width/2-70,110,"Deutsch",255,140,40,true);
        newMenu(width/2-100,height/2-40,"Yes",255,80,80,true);
        newMenu(width/2+20,height/2-40,"No",255,80,80,true);
        newMenu(0,height-40,"",255,80,40,true);
        newMenu(0,height-80,"",255,80,40,true);
        newMenu(width-150,height-50,"Zurück",255,140,40,true);
        newMenu(width-24,50,"",255,25,25,true); //achievements
        newMenu(width-150,height-50,"Zurück",255,140,40,true);
        newMenu(200,10,"",color(25,25,255),80,40,true);                //25
      
    } else if(compareStrings(language,"yolo")) {
        color r = color(255,0,0);
        newMenu(10,10,"What have you done !",r,140,40,true);
        newMenu(width-32,height-32,"",r,32,32,true);
        newMenu(10,60,"€>SàmŒJ¯—àôHò xÇÐû‡éÀu /H¸\\",r,140,40,true);
        newMenu(10,110,"CÙiŒ©è¿—(EUå¥òÌPÄê€Ñ¼vƒu¬.™å'@àFÄ­˜?£. ®r!z+†ðœ    IEND®B`‚",r,140,40,true);
        newMenu(width-65,height-32,"",r,33,32,true);
        newMenu(160,10,"",r,140,40,true);
        newMenu(160,60,"",r,140,40,true);
        newMenu(160,210,"",r,140,40,true);
        newMenu(160,110,"",r,140,40,true);
        newMenu(width/2-70,10,"",r,140,40,true);
        newMenu(width/2-70,60,"",r,140,40,true);
        newMenu(width-150,height-50,"",r,140,40,true);
        newMenu(10,height-50,"",r,100,40,true);
        newMenu(160,160,"",r,140,40,true);
        newMenu(100,10,"",255,140,40,true);
        newMenu(100,60,"",255,140,40,true);
        newMenu(width-180,height-40,"Sauvegarder",255,180,40,true);
        newMenu(width/2-70,110,"",255,140,40,true);
        newMenu(width/2-100,height/2-40,"Yes",255,80,80,true);
        newMenu(width/2+20,height/2-40,"No",255,80,80,true);
        newMenu(0,height-40,"",255,80,40,true);
        newMenu(0,height-80,"",255,80,40,true);
        newMenu(width-150,height-50,"",255,140,40,true);
        newMenu(width-24,50,"",255,25,25,true); //achievements
        newMenu(width-150,height-50,"",255,140,40,true);
        newMenu(200,10,"",color(25,25,255),80,40,true);                //25
      
    } else {
        String[] buttonContent = new String[40];
        for(int i = 0 ; i < buttonContent.length ; i++)
            buttonContent[i] = "Unsuported language";
        
        try {
            file = new File("data/language_"+language);
            FileReader fileReader = new FileReader(file);
            BufferedReader bufferedReader = new BufferedReader(fileReader);
            String line;
            int i = 0;
            while((line = bufferedReader.readLine()) != null && !compareStrings(line,"")) {
                buttonContent[i] = line;
                i++;
            }
            bufferedReader.close();
            fileReader.close();
        } catch(FileNotFoundException e) {
            e.printStackTrace();
            errorMsg("Cannot load this language",SFX[1],e);
        } catch(Exception e) {
            e.printStackTrace();
        }
        
        newMenu(10,10,buttonContent[0],255,200,40,true);
        newMenu(width-32,height-32,"",255,32,32,true);
        newMenu(10,60,buttonContent[1],255,200,40,true);
        newMenu(10,110,buttonContent[2],255,200,40,true);
        newMenu(width-65,height-32,"",255,33,32,true);
        newMenu(260,10,buttonContent[3],255,200,40,true);
        newMenu(260,60,buttonContent[4],255,200,40,true);
        newMenu(260,210,buttonContent[5],255,200,40,true);
        newMenu(260,110,buttonContent[6],255,200,40,true);
        newMenu(width/2-70,10,"Français",255,140,40,true);
        newMenu(width/2-70,60,"English",255,140,40,true);
        newMenu(width-210,height-50,buttonContent[7],255,200,40,true);
        newMenu(10,height-50,buttonContent[8],255,200,40,true);
        newMenu(260,160,buttonContent[9],255,200,40,true);
        newMenu(100,10,"",255,140,40,true);
        newMenu(100,60,"",255,140,40,true);
        newMenu(width-210,height-40,buttonContent[10],255,200,40,true);
        newMenu(width/2-70,110,"Deutsch",255,140,40,true);
        newMenu(width/2-100,height/2-40,"Yes",255,80,80,true);
        newMenu(width/2+20,height/2-40,"No",255,80,80,true);
        newMenu(0,height-40,buttonContent[10],255,80,40,true);
        newMenu(0,height-80,buttonContent[11],255,80,40,true);
        newMenu(width-24,50,"",255,25,25,true); //achievements
        newMenu(0,height-80,buttonContent[12],255,80,40,true);
    }
}