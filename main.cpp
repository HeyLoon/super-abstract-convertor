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

class Cipher {
public:
    virtual std::string process(const std::string& text, int operation) const = 0;
};

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

class CipherWidget : public QWidget {
    Q_OBJECT

public:
    CipherWidget(QWidget* parent = nullptr) : QWidget(parent) {
        setupUI();
        connectSignals();
    }

private slots:
    void onProcessClicked() {
        QString inputText = inputLineEdit->text();
        QString outputText;

        if (cipherTypeComboBox->currentIndex() == 0) {  // Affine Cipher
            outputText = QString::fromStdString(affineCipher.process(inputText.toStdString(), operationComboBox->currentIndex()));
        } else {  // Vigenere Cipher
            outputText = QString::fromStdString(vigenereCipher.process(inputText.toStdString(), operationComboBox->currentIndex()));
        }

        outputLineEdit->setText(outputText);
    }

    void onKannaButtonClicked() {
        QDesktopServices::openUrl(QUrl("https://twitter.com/H_KANNA_0203"));
    }

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

    AffineCipher affineCipher{5, 8};  // Example key for Affine Cipher
    VigenereCipher vigenereCipher{"KEY"};  // Example key for Vigenere Cipher

    void setupUI() {
        mainLayout = new QVBoxLayout(this);

        imageLabel = new QLabel(this);
        QPixmap image(":/ck/chiken.png");
        qreal scaleRatio = 0.5;
        QPixmap scaledImage = image.scaled(image.size() * scaleRatio, Qt::KeepAspectRatio);
        imageLabel->setPixmap(scaledImage);

        player = new QMediaPlayer;
        player->setMedia(QUrl("qrc:/ck/music.wav"));
        player->setVolume(10);
        player->play();

        titleLabel = new QLabel("段考爆破神器", this);
        titleLabel->setAlignment(Qt::AlignCenter);

        cipherTypeComboBox = new QComboBox(this);
        cipherTypeComboBox->addItem("Affine cipher");
        cipherTypeComboBox->addItem("Vigenère cipher");

        operationComboBox = new QComboBox(this);
        operationComboBox->addItem("搞他!");
        operationComboBox->addItem("解他!");

        inputLineEdit = new QLineEdit(this);
        inputLineEdit->setPlaceholderText("用滿滿的文字填滿這裡...");

        outputLineEdit = new QLineEdit(this);
        outputLineEdit->setReadOnly(true);
        outputLineEdit->setPlaceholderText("結果是...");

        processButton = new QPushButton("簽下去", this);
        KannaButton = new QPushButton("寶🥰", this);
        KannaBookButton = new QPushButton("橋本純情內科🥰", this);

        QHBoxLayout* comboLayout = new QHBoxLayout;
        comboLayout->addWidget(cipherTypeComboBox);
        comboLayout->addWidget(operationComboBox);

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

    void connectSignals() {
        connect(processButton, &QPushButton::clicked, this, &CipherWidget::onProcessClicked);
        connect(KannaButton, &QPushButton::clicked, this, &CipherWidget::onKannaButtonClicked);
        connect(KannaBookButton, &QPushButton::clicked, this, &CipherWidget::onKannaBookButtonClicked);
    }
};

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
