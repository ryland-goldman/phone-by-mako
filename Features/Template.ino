/*
 *
 * This is the template for submitting new features.
 * When testing, add this to the bottom of MakoSystem.ino and add item to dispMenu() and function to keyPress()
 *
 *
 */
 
void displayFunction(){
  u8g.printPos(0,0);
  u8g.print("Hello, World!");
}
 
void functionName() {
  //Function called on load of image
  while (key != '#')
  {
  
     /* Put your code here */
    
    key = keypad.getKey();
    if(key == '#'){
      break;
    }
  }
  do {
      displayFunction();
    } while( u8g.nextPage() );
}
