# VigenereCipher及AffineCipher功能實現


## 前言
凱薩密碼（Caesar Cipher）是歷史上最早的密碼之一，它簡單的將字母按照固定的位移量進行替換來達到加密的目的。雖然凱薩密碼在古代用途廣泛，但由於其易受到頻率分析攻擊的弱點，現代密碼學中較少使用。


**Vigenere Cipher**和**Affine Cipher**是兩種進階的古典密碼學算法，它們在歷史上代表了對凱薩密碼的改進和擴展。這份報告將深入探討這兩種算法的功能實現、應用以及未來的發展方向。


## 內文
### 2.1 Vigenere Cipher
Vigenere Cipher是在16世紀由布拉德沃德提出的，它使用一個稱為「密鑰字」的短句來進行加密。密文的每個字母都是由明文中的相應字母和密鑰字中的字母組成的。這使得Vigenere Cipher相對於凱薩密碼更難被破解，因為它引入了密鑰的概念，增加了加密的隨機性。
自本次報告中，使用了以下程式碼實現Vigenere Cipher:
```c++
class VigenereCipher : public Cipher {
private:
    std::string key;

public:
    VigenereCipher(const std::string& k) : key(k) {}

    std::string process(const std::string& text, int operation) const override {
        if (operation == 0) {  // Encrypt
            return encrypt(text);
        } else {  // Decrypt
            return decrypt(text);
        }
    }

private:
    std::string encrypt(const std::string& plaintext) const {
        std::string ciphertext = "";
        for (size_t i = 0; i < plaintext.length(); ++i) {
            if (isalpha(plaintext[i])) {
                char base = isupper(plaintext[i]) ? 'A' : 'a';
                char offset = key[i % key.length()] - 'A';
                ciphertext += static_cast<char>((plaintext[i] + offset - base) % 26 + base);
            } else {
                ciphertext += plaintext[i];
            }
        }
        return ciphertext;
    }

    std::string decrypt(const std::string& ciphertext) const {
        std::string decryptedText = "";
        for (size_t i = 0; i < ciphertext.length(); ++i) {
            if (isalpha(ciphertext[i])) {
                char base = isupper(ciphertext[i]) ? 'A' : 'a';
                char offset = key[i % key.length()] - 'A';
                decryptedText += static_cast<char>((ciphertext[i] - offset - base + 26) % 26 + base);
            } else {
                decryptedText += ciphertext[i];
            }
        }
        return decryptedText;
    }
};
```
### 2.1 Affine Cipher
Affine Cipher是一種更加複雜的古典密碼，它在每個字母上應用了一個仿射函數。這個函數具有兩個參數a和b，其中a和26互質。Affine Cipher的強大之處在於其線性轉換特性，使其能夠抵抗基本的頻率分析攻擊。
自本次報告中，使用了以下程式碼實現Vigenere Cipher:
```c++
class AffineCipher : public Cipher {
private:
    int keyA;
    int keyB;

public:
    AffineCipher(int a, int b) : keyA(a), keyB(b) {}

    std::string process(const std::string& text, int operation) const override {
        if (operation == 0) {  // Encrypt
            return encrypt(text);
        } else {  // Decrypt
            return decrypt(text);
        }
    }

private:
    std::string encrypt(const std::string& plaintext) const {
        std::string ciphertext = "";
        for (char ch : plaintext) {
            if (isalpha(ch)) {
                char base = isupper(ch) ? 'A' : 'a';
                ch = static_cast<char>((keyA * (ch - base) + keyB) % 26 + base);
            }
            ciphertext += ch;
        }
        return ciphertext;
    }

    std::string decrypt(const std::string& ciphertext) const {
        int modInverseA = modInverse(keyA, 26);
        std::string plaintext = "";
        for (char ch : ciphertext) {
            if (isalpha(ch)) {
                char base = isupper(ch) ? 'A' : 'a';
                ch = static_cast<char>(modInverseA * (ch - base - keyB + 26) % 26 + base);
            }
            plaintext += ch;
        }
        return plaintext;
    }

    int modInverse(int a, int m) const {
        a = a % m;
        for (int x = 1; x < m; x++)
            if ((a * x) % m == 1)
                return x;
        return 1;
    }
};
```

### 2.3 比較
Vigenere Cipher和Affine Cipher相較於凱薩密碼都提供了更高的安全性。Vigenere Cipher通過引入密鑰字，增加了對抗頻率分析攻擊的能力。而Affine Cipher則透過線性轉換的特性，增加了密文的隨機性。兩者雖然都是古典密碼學中的重要算法，但在現代密碼學中過於容易遭到破解。至今主要用作教學和歷史研究的對象。


| 特性 | Vigenere Cipher	 | Affine Cipher |
| -------- | -------- | -------- |
| 加密方式	     | 多表雜湊加密，使用密鑰字進行加密     | 單一雜湊加密，使用仿射函數進行加密     |
|密鑰管理	|密鑰長度與明文相同|使用兩個整數a和b作為密鑰|
|抗頻率分析	|較強，密鑰字增加了加密的隨機性	|較弱，容易受到頻率分析攻擊的影響|
|運算方式|字母和密鑰字的相加，使用模運算	|仿射函數的線性轉換，涉及乘法和加法|
|安全性	|普通，對抗基本攻擊，但仍可被破解|較強，對抗頻率分析有一定的抵抗力|
|應用場景	|簡短文本，需要較高的加密強度	|對抗基本攻擊，需求較複雜的加密|

### 2.4 運用C++及qt實現Vigenere Cipher及Affine Cipher

以下長度偏長，可以先來看結果長怎樣:)
[**專案連結(Github)**](https://github.com/HeyLoon/super-abstract-convertor) (release裡面也可以找到編譯完的檔案)
**[編譯後檔案直接下載連結(Google Drive)](https://drive.google.com/file/d/1xox37nsL5B9OyOR-vnaHusHRzw3trmCE/view?usp=drive_link)**
如果要自己編譯，請使用qt creator並確保版本於5.9.8或以上

```C++
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QFont>
#include <QString>
#include <QDesktopServices>
#include <QUrl>
#include <QMediaPlayer>
#include <iostream>
#include <string>

// Abstract base class for different cipher implementations
class Cipher {
public:
    virtual std::string process(const std::string& text, int operation) const = 0;
};

// Implementation of the Affine Cipher, a type of substitution cipher
class AffineCipher : public Cipher {
private:
    int keyA;
    int keyB;

public:
    AffineCipher(int a, int b) : keyA(a), keyB(b) {}

    std::string process(const std::string& text, int operation) const override {
        if (operation == 0) {  // Encrypt
            return encrypt(text);
        } else {  // Decrypt
            return decrypt(text);
        }
    }

private:
    std::string encrypt(const std::string& plaintext) const {
        std::string ciphertext = "";
        for (char ch : plaintext) {
            if (isalpha(ch)) {
                char base = isupper(ch) ? 'A' : 'a';
                ch = static_cast<char>((keyA * (ch - base) + keyB) % 26 + base);
            }
            ciphertext += ch;
        }
        return ciphertext;
    }

    std::string decrypt(const std::string& ciphertext) const {
        int modInverseA = modInverse(keyA, 26);
        std::string plaintext = "";
        for (char ch : ciphertext) {
            if (isalpha(ch)) {
                char base = isupper(ch) ? 'A' : 'a';
                ch = static_cast<char>(modInverseA * (ch - base - keyB + 26) % 26 + base);
            }
            plaintext += ch;
        }
        return plaintext;
    }

    int modInverse(int a, int m) const {
        a = a % m;
        for (int x = 1; x < m; x++)
            if ((a * x) % m == 1)
                return x;
        return 1;
    }
};

// Implementation of the Vigenere Cipher, another type of substitution cipher
class VigenereCipher : public Cipher {
private:
    std::string key;

public:
    VigenereCipher(const std::string& k) : key(k) {}

    std::string process(const std::string& text, int operation) const override {
        if (operation == 0) {  // Encrypt
            return encrypt(text);
        } else {  // Decrypt
            return decrypt(text);
        }
    }

private:
    std::string encrypt(const std::string& plaintext) const {
        std::string ciphertext = "";
        for (size_t i = 0; i < plaintext.length(); ++i) {
            if (isalpha(plaintext[i])) {
                char base = isupper(plaintext[i]) ? 'A' : 'a';
                char offset = key[i % key.length()] - 'A';
                ciphertext += static_cast<char>((plaintext[i] + offset - base) % 26 + base);
            } else {
                ciphertext += plaintext[i];
            }
        }
        return ciphertext;
    }

    std::string decrypt(const std::string& ciphertext) const {
        std::string decryptedText = "";
        for (size_t i = 0; i < ciphertext.length(); ++i) {
            if (isalpha(ciphertext[i])) {
                char base = isupper(ciphertext[i]) ? 'A' : 'a';
                char offset = key[i % key.length()] - 'A';
                decryptedText += static_cast<char>((ciphertext[i] - offset - base + 26) % 26 + base);
            } else {
                decryptedText += ciphertext[i];
            }
        }
        return decryptedText;
    }
};

// Widget for the graphical user interface
class CipherWidget : public QWidget {
    Q_OBJECT

public:
    CipherWidget(QWidget* parent = nullptr) : QWidget(parent) {
        setupUI();
        connectSignals();
    }

private slots:
    // Slot for the process button click
    void onProcessClicked() {
        QString inputText = inputLineEdit->text();
        QString outputText;
        // Determine the selected cipher type and perform the corresponding operation
        if (cipherTypeComboBox->currentIndex() == 0) {  // Affine Cipher
            outputText = QString::fromStdString(affineCipher.process(inputText.toStdString(), operationComboBox->currentIndex()));
        } else {  // Vigenere Cipher
            outputText = QString::fromStdString(vigenereCipher.process(inputText.toStdString(), operationComboBox->currentIndex()));
        }

        outputLineEdit->setText(outputText);
    }

    // Slot for the Kanna button click, opens my wife's Twitter link
    void onKannaButtonClicked() {
        QDesktopServices::openUrl(QUrl("https://twitter.com/H_KANNA_0203"));
    }

    // Slot for the Kanna button click, opens my wife's Official Page link
    void onKannaBookButtonClicked() {
        QDesktopServices::openUrl(QUrl("https://fc.hashimotokanna.jp/"));
    }

private:
    QVBoxLayout* mainLayout;
    QLabel* titleLabel;
    QMediaPlayer* player;
    QComboBox* cipherTypeComboBox;
    QComboBox* operationComboBox;
    QLineEdit* inputLineEdit;
    QLineEdit* outputLineEdit;
    QPushButton* processButton;
    QPushButton* KannaButton;
    QPushButton* KannaBookButton;
    QLabel* imageLabel;

    AffineCipher affineCipher{5, 8};  // Example key for Affine Cipher, you can change key here ny yourself
    VigenereCipher vigenereCipher{"KEY"};  // Example key for Vigenere Cipher, You can change key here ny yourself

    // Set up the user interface components
    void setupUI() {
        mainLayout = new QVBoxLayout(this);

        // Set up and scale the image label
        imageLabel = new QLabel(this);
        QPixmap image(":/ck/chiken.png");
        qreal scaleRatio = 0.5;
        QPixmap scaledImage = image.scaled(image.size() * scaleRatio, Qt::KeepAspectRatio);
        imageLabel->setPixmap(scaledImage);

        // Set up the media player for background music
        player = new QMediaPlayer;
        player->setMedia(QUrl("qrc:/ck/music.wav"));
        player->setVolume(10);
        player->play();

        // Set up the title label
        titleLabel = new QLabel("段考爆破神器", this);
        titleLabel->setAlignment(Qt::AlignCenter);

        // Set up combo boxes for selecting cipher type and operation
        cipherTypeComboBox = new QComboBox(this);
        cipherTypeComboBox->addItem("Affine cipher");
        cipherTypeComboBox->addItem("Vigenère cipher");

        operationComboBox = new QComboBox(this);
        operationComboBox->addItem("搞他!");
        operationComboBox->addItem("解他!");

        // Set up input and output text fields
        inputLineEdit = new QLineEdit(this);
        inputLineEdit->setPlaceholderText("用滿滿的文字填滿這裡...");

        outputLineEdit = new QLineEdit(this);
        outputLineEdit->setReadOnly(true);
        outputLineEdit->setPlaceholderText("結果是...");

        // Set up buttons for encryption/decryption and external links
        processButton = new QPushButton("簽下去", this);
        KannaButton = new QPushButton("寶🥰", this);
        KannaBookButton = new QPushButton("橋本純情內科🥰", this);

        // Set up layout for cipher type and operation combo boxes
        QHBoxLayout* comboLayout = new QHBoxLayout;
        comboLayout->addWidget(cipherTypeComboBox);
        comboLayout->addWidget(operationComboBox);

        // Add all components to the main layout
        mainLayout->addWidget(imageLabel);
        mainLayout->addWidget(titleLabel);
        mainLayout->addLayout(comboLayout);
        mainLayout->addWidget(inputLineEdit);
        mainLayout->addWidget(outputLineEdit);
        mainLayout->addWidget(processButton);
        mainLayout->addWidget(KannaButton);
        mainLayout->addWidget(KannaBookButton);

        setLayout(mainLayout);
    }

    // Connect signals (events) to corresponding slots (functions)
    void connectSignals() {
        connect(processButton, &QPushButton::clicked, this, &CipherWidget::onProcessClicked);
        connect(KannaButton, &QPushButton::clicked, this, &CipherWidget::onKannaButtonClicked);
        connect(KannaBookButton, &QPushButton::clicked, this, &CipherWidget::onKannaBookButtonClicked);
    }
};

// Main function
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QFont font("Microsoft YaHei", 12);
    QApplication::setFont(font);
    CipherWidget cipherWidget;
    cipherWidget.setWindowTitle("哈哈哈段考爆了 - 21210 李品翰 (2006-2024)");
    cipherWidget.show();
    return app.exec();
}

#include "main.moc"
```

[**專案連結(Github)**](https://github.com/HeyLoon/super-abstract-convertor)
**[編譯後檔案直接下載連結(Google Drive)](https://drive.google.com/file/d/1xox37nsL5B9OyOR-vnaHusHRzw3trmCE/view?usp=drive_link)**

## 結論與討論
Vigenere Cipher和Affine Cipher都有其獨特的優勢，Vigenere Cipher適用於需要較高加密強度的應用，例如保護短語或簡訊。Affine Cipher則在對抗一些基本攻擊方面表現優越，可以應用於一些較為複雜的加密需求。

未來的改進方向可以包括對這些算法進行更多的分析，並嘗試應用於現代密碼學的某些方面。例如，可以考慮結合這些算法與現代加密技術，以提高整體安全性。

## 參考文獻
Qt project. (n.d.). Qt Documentation. Retrieved January 15, 2024, from https://doc.qt.io/
C++. (n.d.). C++ Documentation. Retrieved January 15, 2024, from https://devdocs.io/cpp/
Wikipedia contributors. (n.d.). Vigenère Cipher - Wikipedia. Retrieved January 15, 2024, from https://en.wikipedia.org/wiki/Vigen%C3%A8re_cipher
Wikipedia contributors. (n.d.). Affine Cipher - Wikipedia. Retrieved January 15, 2024, from https://en.wikipedia.org/wiki/Affine_cipher
史博勳. (2022, June 27). 二.內文 - HackMD. Retrieved January 15, 2024, from https://hackmd.io/@bEguM6t5RgyX3mvN42fwCA/S1eHzIaK5