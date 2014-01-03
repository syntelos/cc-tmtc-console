/*
 * When bound to signal "Window::init" this example script will popup
 * a message box when the application starts.
 */
function init(){
    /*
     * Because Main/Window is the global object, and the alert
     * method is a public slot on the Window, the following evaluates
     * to a call to "Window::alert()".
     */
    alert("Alert Example Title", "Alert example message body");
}
