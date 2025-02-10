
# AES-Encrypted Authentication System

## 📖 Overview

This project implements a simple authentication system using **AES encryption (CBC mode)** to securely store and validate user credentials. It features a **terminal-based user interface** built with `ncurses`, allowing users to enter their **username and password** and verify their credentials securely.

## 🎯 Features

- **AES-128 Encryption**: Securely encrypts and decrypts user credentials.
- **CBC Mode Encryption**: Prevents pattern recognition in ciphertext.
- **Terminal-Based UI**: Uses `ncurses` to create an interactive authentication interface.
- **Login System**: Verifies credentials against stored encrypted values.
- **Secure Memory Handling**: Ensures no plaintext credentials remain in memory longer than necessary.

## 🛠️ Setup & Installation

### 1️⃣ Prerequisites

Make sure your system has the following installed:

- **GCC (GNU Compiler Collection)**
- **`ncurses` library**

#### Install dependencies:

**For Linux (Ubuntu/Debian-based systems):**
```bash
sudo apt update
sudo apt install build-essential libncurses5-dev libncursesw5-dev
```

**For macOS (using Homebrew):**
```bash
brew install ncurses
```

**For Windows:**  
Use **WSL (Windows Subsystem for Linux)** or an IDE like **Code::Blocks**.

---

### 2️⃣ Cloning the Repository

```bash
git clone https://github.com/yourusername/aes-authentication.git
cd aes-authentication
```

---

### 3️⃣ Compilation

Run the following command to compile the project:

```bash
gcc -o cryptography_app oldschool.c aes.c -lncurses
```

---

### 4️⃣ Running the Program

After compilation, execute the program:

```bash
./cryptography_app
```

---

## 📌 Usage

1. Launch the program.
2. Enter your **username** and **password**.
3. If the credentials match the stored encrypted values, you will be authenticated successfully.
4. If incorrect credentials are provided, access will be denied.

**Default Credentials (for testing)**  
- **Username:** `username12345678`
- **Password:** `1234567812345678`

⚠️ *These credentials are hardcoded for demonstration and should be replaced with dynamic storage in production.*

---

## 📸 Screenshots

![WhatsApp Image 2025-02-10 at 21 04 16_1bc8b630](https://github.com/user-attachments/assets/bdaca809-ca71-44c9-8f02-65bf0438fe4a)
![WhatsApp Image 2025-02-10 at 21 04 16_c1bbe619](https://github.com/user-attachments/assets/b00794c4-7347-4cf9-961d-0817cd1ebc97)
![image](https://github.com/user-attachments/assets/0b647af4-a6ff-4e6b-b535-b9dec9058b37)
![WhatsApp Image 2025-02-10 at 21 04 15_0b4c5c64](https://github.com/user-attachments/assets/8cbcc58b-d89c-4230-9151-c8a7437cd9e7)


---

## 🏗️ Project Structure

```
aes-authentication/
├── aes.c          # AES encryption and decryption logic
├── aes.h          # AES header file (function declarations)
├── oldschool.c    # Main authentication logic with ncurses UI
├── README.md      # Project documentation
└── Makefile       # (Optional) Build automation script
```

---

## 🔍 How It Works

1. **AES Key and IV Initialization**  
   - The program initializes an AES context with a **16-byte encryption key and IV**.

2. **Credential Encryption**  
   - The default username and password are **encrypted** and stored in memory.

3. **User Authentication**  
   - The user inputs their username and password.
   - The input is encrypted and compared with stored encrypted credentials.
   - If they match, authentication succeeds; otherwise, it fails.

---

## 💡 Improvements & Future Enhancements

- ✅ Implement **dynamic key generation** instead of hardcoded keys.
- ✅ Support **hashed password storage** instead of encryption.
- ✅ Store credentials securely using **databases** instead of in-memory storage.
- ✅ Implement **two-factor authentication (2FA)** for added security.

---

## 📜 License

This project is licensed under the **MIT License**. Feel free to modify and distribute it.

---

## 🤝 Contributing

Contributions are welcome! If you'd like to improve the project:

1. Fork the repository 🍴
2. Create a new branch 🔀
3. Commit your changes 🎯
4. Submit a Pull Request 📨

---

## 📬 Contact

For any queries, reach out via:

📧 Email: your.email@example.com  
🐙 GitHub: [yourusername](https://github.com/yourusername)  

---

🚀 **Happy Encrypting!**
```

---

This `README.md` file provides a structured, user-friendly, and professional guide for your project. Let me know if you want any modifications! 🚀
