/*
 * When bound to signal "Window::init" this example script will popup
 * the configuration dialog when the application starts.
 */
function init(){
    /*
     * Because Main/Window is the global object, and the configure
     * method is a public slot on the Window, the following evaluates
     * to a call to "Window::configure()".
     */
    configure();
}
