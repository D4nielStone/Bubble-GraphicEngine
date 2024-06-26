#include "entidade.h"

namespace Bubble {
	namespace Entidades {

		Entidade::Entidade(Bubble::Arquivadores::Arquivo3d arquivo_objeto)
			: transformacao(std::make_shared<Bubble::Componentes::Transformacao>()) { // Use std::make_shared aqui
			adicionarComponente(transformacao);
			carregarModelo(std::move(arquivo_objeto));
		}
		Entidade::Entidade() : transformacao(std::make_shared<Bubble::Componentes::Transformacao>()) {
			adicionarComponente(std::make_shared<Bubble::Componentes::Transformacao>());
		};
		void Entidade::atualizar(float deltaTime) {
			for (auto& c : Componentes) {
				c->atualizar(deltaTime);  // Certifique-se de que atualizar recebe deltaTime como par�metro
			}
		}

		const char* Entidade::nome() {
			return Nome;
		}

		void Entidade::carregarModelo(Bubble::Arquivadores::Arquivo3d object_file) {
			int m = 0;
			for (const auto& vertex : object_file.vertices) {
				if (m < object_file.materiais.size()) {
					auto renderizador = std::make_shared<Bubble::Componentes::Renderizador>(vertex, object_file.materiais[m]);
					adicionarComponente(renderizador);
					m++;
				}
			}
		}

		Bubble::Comum::Componente& Entidade::obterComponente(const std::string& nome) {
			for (auto& c : Componentes) {
				if (c->nome() == nome) {
					return *c;
				}
			}
			throw std::runtime_error("Componente n�o encontrado: " + nome);
		}

		std::vector<std::reference_wrapper<Bubble::Comum::Componente>> Entidade::obterComponentes(const std::string& nome) {
			std::vector<std::reference_wrapper<Bubble::Comum::Componente>> comps;
			for (auto& c : Componentes) {
				if (c->nome() == nome) {
					comps.push_back(*c);
				}
			}
			return comps;
		}

		std::shared_ptr<Bubble::Componentes::Transformacao> Entidade::obterTransformacao() {
			return transformacao;
		}

		const std::vector<std::shared_ptr<Bubble::Comum::Componente>>& Entidade::listaDeComponentes() const {
			return Componentes;
		}

		void Entidade::adicionarComponente(std::shared_ptr<Bubble::Comum::Componente> componente) {
			componente->definirPai(this);
			Componentes.push_back(componente);
		}

	}
}
