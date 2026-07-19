#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

// Variáveis globais para o controle das tentativas
int limiteTentativas = 10; 
int tentativasAtuais = 0;

// Janela personalizada do Mod
class SelectorMenuPopup : public FLAlertLayer, public TextInputDelegate {
public:
    TextInput* m_inputCampo;

    static SelectorMenuPopup* create() {
        auto ret = new SelectorMenuPopup();
        if (ret && ret->init(240, 320, "blank.png", "Salvar")) { // O botão padrão vira "Salvar"
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool init(float width, float height, const char* bg, const char* btn) {
        if (!FLAlertLayer::init(width, height, bg, btn)) return false;

        auto tamanhoTela = CCDirector::sharedDirector()->getWinSize();
        auto painelPrincipal = this->m_mainLayer;

        // 1. Caixa Amarela de Fundo
        auto caixaMenu = CCSprite::create("caixa_menu.png");
        caixaMenu->setPosition({ tamanhoTela.width / 2, tamanhoTela.height / 2 });
        painelPrincipal->addChild(caixaMenu);

        // 2. Padrão Vermelho por dentro
        auto fundoPattern = CCSprite::create("fundo_pattern.png");
        fundoPattern->setPosition({ tamanhoTela.width / 2, tamanhoTela.height / 2 });
        painelPrincipal->addChild(fundoPattern);

        // 3. Título "ATTEMPTS SELECTOR"
        auto titulo = CCSprite::create("titulo_selector.png");
        titulo->setPosition({ tamanhoTela.width / 2, (tamanhoTela.height / 2) + 110 });
        painelPrincipal->addChild(titulo);

        // 4. Criando o Campo de Texto (Input)
        // Criamos um campo com largura 120, altura 30, texto de fundo e fonte padrão
        m_inputCampo = TextInput::create(120, "Número", "chatFont.fnt");
        m_inputCampo->setPosition({ tamanhoTela.width / 2, tamanhoTela.height / 2 });
        
        // Configura para aceitar apenas caracteres numéricos (0-9)
        m_inputCampo->setFilter("0123456789");
        m_inputCampo->setMaxCharCount(4); // Limita a até 9999 tentativas
        
        // Define o valor atual já preenchido para facilitar
        m_inputCampo->setString(std::to_string(limiteTentativas));
        
        // Ativa o delegate para podermos capturar o que é digitado
        m_inputCampo->setDelegate(this);
        painelPrincipal->addChild(m_inputCampo);

        // Reposiciona o botão "Salvar" na parte inferior da caixa
        if (m_buttonMenu) {
            m_buttonMenu->setPosition({ tamanhoTela.width / 2, (tamanhoTela.height / 2) - 100 });
        }

        return true;
    }

    // Função executada quando o jogador digita ou fecha o teclado
    void textChanged(TextInput* input) override {
        std::string texto = input->getString();
        if (!texto.empty()) {
            // Converte o texto digitado em número inteiro e salva
            limiteTentativas = std::stoi(texto);
            
            // Evita que o jogador coloque 0 tentativas e quebre o jogo
            if (limiteTentativas <= 0) {
                limiteTentativas = 1;
            }
        }
    }
};

// Código para injetar o Botão de abrir no Menu Principal
class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        auto tamanhoTela = CCDirector::sharedDirector()->getWinSize();

        auto spriteBotao = CCSprite::create("botao_abrir.png");
        auto btnSelector = CCMenuItemSpriteExtra::create(
            spriteBotao, this, menu_selector(MyMenuLayer::onOpenSelector)
        );

        auto menuMod = CCMenu::create();
        menuMod->addChild(btnSelector);
        menuMod->setPosition({ 45, 45 });
        this->addChild(menuMod);

        return true;
    }

    void onOpenSelector(CCObject* sender) {
        SelectorMenuPopup::create()->show();
    }
};

// Código de lógica dentro das fases (PlayLayer)
class $modify(MyPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontRunActions) {
        if (!PlayLayer::init(level, useReplay, dontRunActions)) return false;
        
        if (this->m_isPracticeMode == false && m_currentAttempt == 1) {
            tentativasAtuais = 0;
        }
        return true;
    }

    void resetLevel() {
        PlayLayer::resetLevel();
        tentativasAtuais++;

        if (tentativasAtuais >= limiteTentativas) {
            FLAlertLayer::create("Limite Atingido!", "Você alcançou o máximo de tentativas.", "OK")->show();
            this->onQuit(nullptr); 
        }
    }
};
