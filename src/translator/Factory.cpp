#include "translator/Factory.h"
#include <algorithm>

// List of exported translator.
TranslatorPtr FrameTranslator();
TranslatorPtr GrammarTranslator();
TranslatorPtr GraphDAGTranslator();
TranslatorPtr GraphUndirectedTranslator();
TranslatorPtr GraphPlanarTranslator();
TranslatorPtr MathTranslator();
TranslatorPtr SequenceTranslator();
TranslatorPtr TableTranslator();
TranslatorPtr TreeTranslator();
TranslatorPtr FlowchartTranslator();

std::vector<TranslatorPtr>& TranslatorList() {
  static std::vector<TranslatorPtr> out;
  if (out.size() != 0)
    return out;

  out.push_back(MathTranslator());
  out.push_back(SequenceTranslator());
  out.push_back(TreeTranslator());
  out.push_back(TableTranslator());
  out.push_back(GrammarTranslator());
  out.push_back(FrameTranslator());
  out.push_back(GraphDAGTranslator());
  out.push_back(GraphUndirectedTranslator());
  out.push_back(GraphPlanarTranslator());
  out.push_back(FlowchartTranslator());

  auto is_null = [](const TranslatorPtr& t) { return t == nullptr; };
  out.erase(std::remove_if(out.begin(), out.end(), is_null), out.end());

  return out;
}

Translator* FindTranslator(const std::string& name) {
  auto& list = TranslatorList();
  for (auto& it : list) {
    if (it->Identifier() == name)
      return it.get();
  }
  return nullptr;
}
