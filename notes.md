there is no convenient mechanism to debug why connection fails as described in [[this thread]][def]  
according to [the Firefox source code][def2] theese errors are generated :
- `No such native application ${application}` - in the constructor
- `Native application tried to send a message of ${len} bytes, which exceeds the limit of ${lazy.maxRead} bytes.` - on reading
- `Attempt to postMessage on disconnected port` - on sending
- `Write too big` - on sending

[def]: https://discourse.mozilla.org/t/native-messaging-api-catching-a-connection-request-failure-versus-a-genuine-disconnection/72562
[def2]: https://searchfox.org/mozilla-central/source/toolkit/components/extensions/NativeMessaging.sys.mjs