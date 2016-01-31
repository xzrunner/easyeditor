#ifndef _EASYTEXTURE_SYMBOL_CONTAINER_H_
#define _EASYTEXTURE_SYMBOL_CONTAINER_H_



namespace etexture
{

class Symbol;

class SymbolContainer : public ee::DataContainer
{
public:
	SymbolContainer(Symbol* symbol);
	virtual ~SymbolContainer();

	//
	// DataContainer interface
	//
	virtual void Traverse(ee::Visitor& visitor, bool order = true) const;
	virtual void Traverse(ee::Visitor& visitor, ee::DataTraverseType type = ee::DT_ALL, bool order = true) const;
	virtual bool Remove(Object* obj);
	virtual bool Insert(Object* obj);
	virtual bool Insert(Object* obj, int idx);
	virtual bool Clear();
	virtual bool ResetOrder(const Object* obj, bool up);
	virtual bool ResetOrderMost(const Object* obj, bool up);

private:
	Symbol* m_symbol;

}; // SymbolContainer

}

#endif // _EASYTEXTURE_SYMBOL_CONTAINER_H_