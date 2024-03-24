let debug = true;

let appPort = browser.runtime.connectNative("browser_window_popup");

let popupWindowId = null;

appPort.onMessage.addListener((message) => {
    console.log("received : " + JSON.stringify(message));

    // noinspection JSIncompatibleTypesComparison
    if (message === "handshake")
        return;

    let {text} = message;
    if (text === undefined || text === "" || typeof text !== "string") {
        console.log("error : invalid message");
        return;
    }
    text = text.trim();
    console.log("text : " + text);
    if (text.length > 2000) {
        console.log("error : text is too long");
        return;
    }

    let {x: cursorX, y: cursorY} = message;
    console.log("cursor : " + cursorX + " " + cursorY);

    browser.storage.local.get().then(storage => {
        let {params, width, height} = storage;

        let url = `https://translate.google.com/details?text=${encodeURIComponent(text)}${params}`;
        console.log("url : " + url);

        let createData = {
            type: "popup",
            url: url,
            left: cursorX,
            top: cursorY,
            width: width,
            height: height,
        };
        console.log("createData : " + JSON.stringify(createData));

        if (debug)
            return;

        if (popupWindowId !== null) {
            browser.windows.remove(popupWindowId);
            popupWindowId = null;
        }

        let creating = browser.windows.create(createData);
        creating.then((window) => {
                console.log(`the popup has been created ${window.id}`);
                popupWindowId = window.id;
            },
            console.log);
    });
});

appPort.onDisconnect.addListener((port) => {
    console.log(`disconnected ${port.error ? port.error : ''}`);
});

browser.windows.onFocusChanged.addListener((windowId) => {
    if (popupWindowId !== null) {
        console.log(`closing popup : ${popupWindowId}`);
        browser.windows.remove(popupWindowId).then(
            () => null,
            (error) => null // mute error when popup is already closed by user
        );
        popupWindowId = null;
    }
});

browser.storage.local.get().then(storage => {
    let {params, width, height} = storage;
    if (params === undefined) params = "&op=translate&sl=en&tl=uk"; // https://stackoverflow.com/questions/26714426
    if (width === undefined) width = 740;
    if (height === undefined) height = 700;
    browser.storage.local.set({params, width, height});
});

browser.browserAction.onClicked.addListener(() => {
    browser.runtime.openOptionsPage();
});

appPort.postMessage("handshake")
