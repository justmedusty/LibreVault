# Citadel

## Key value encrypted storage
5 levels of the vault: <br>
DEFCON1 -> Most secure, use strongest password <br>
DEFCON2 -> Very secure, use very strong password <br>
DEFCON3 -> Middle ground, use secure password, can store less important things like a forum password <br>
DEFCON4 -> Less secure, can use weaker passwords <br>
DEFCON5 -> Least secure, can use a simple password, mostly to keep people from snooping<br>
AES 256 GCM will be used for the HMAC-like tag to verify that your password is correct.<br>
There will be a signature within each DEFCON level that will securely identify your password(a known string encrypted)<br>
This will ensure that you do not accidentally type a different password for another entry within that DEFCON level
