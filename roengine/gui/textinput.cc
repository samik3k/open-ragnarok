/* $Id$ */
#include "stdafx.h"

#include "roengine/gui/textinput.h"
#include "roengine/gui/gui.h"
#include "sdle/font.h"
#include "sdle/ft_font.h"

#include <stdlib.h>

unsigned int GUI::TextInput::cursor_delay = 500;

GUI::TextInput::~TextInput() {
}

GUI::TextInput::TextInput(Element* parent, const TiXmlElement* e, CacheManager& cache) : Label(parent, e, cache) {
	m_focusable = true;
	m_delay = 0;
	m_pos = 0;
	m_start = 0;
	m_bar = true;
	actived = false;
	SelectFlag = false;
	m_key = SDLK_UNKNOWN;

	selStart = selEnd = 0;
	len = 0;

	m_MouseDown = false;

	G_Text.Surface = SDL_GetWindowSurface(SDL_GetWindowFromID(0));
	G_Text.Text_Changed	= false;
	if( MaxLen <= 0 ) {
		MaxLen = 0;
	}
	G_Text.Len		= MaxLen;
	G_Text.x        = pos_x;
	G_Text.y        = pos_y + 5;	// By Naxan
	G_Text.z        = 0;
	G_Text.EffectWidth = w;
	G_Text.FColor   = SDL_BLACK;
	G_Text.BColor   = SDL_WHITE;
	G_Text.SFColor   = SDL_WHITE;
	G_Text.SBColor   = SDL_BLUE;
	G_Text.Alpha    = SDL_OPACITY;
	G_Text.m_text   = (Uint16*)calloc(MaxLen + 1, sizeof(Uint16));

	// TODO: put this in the FontManager
	//sdle::FTFont *font = new sdle::FTFont();
	//G_Text.Font = font;
	//font->open("data\\font\\lsans.ttf", 12);

	G_Text.Head = G_Text.Start  = G_Text.Current = G_Text.select1 = G_Text.select2 = NULL;// G_Text.End = NewNode(NULL, NULL, AU('\0'));
}

void GUI::TextInput::Draw(unsigned int delay) {
	int x = 0;
	int tmp_pos = 0;

	GUI::Gui& gui = GUI::Gui::getSingleton();

	m_delay += delay;

	//GetStringFromNode();
	float color[4];
	glGetFloatv(GL_CURRENT_COLOR, color);

	if (1) {
		glDisable(GL_TEXTURE_2D);

		//glColor4f(0, 0.1, 1, 0.6f);
#if 0
	/*	bool sel = false;
		for (int i = 0; i < G_Text.Len; i++) {
			if (sel && !G_Text.Start[i].selected)
				break;
			if (G_Text.Start[i].selected)
				sel = true;
		}*/
		glColor4f(0.482352941f, 0.580392157f, 0.803921569f, color[3]);
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glBegin(GL_QUADS);
		glVertex3i(x+selStart, 3, 0);
		glVertex3i(x+selStart, 15, 0);
		glVertex3i(x+selEnd*6, 15, 0);
		glVertex3i(x+selEnd*6, 3, 0);
		glEnd();
		//glDisable(GL_BLEND);
#endif
		glColor4fv(color);

		glEnable(GL_TEXTURE_2D);
	}

	gui.TextOutEx(&G_Text);

	x = GetCursorX();

	while (m_delay > cursor_delay) {
		m_delay -= cursor_delay;
		m_bar = !m_bar;
	}
	if (m_bar && actived) {
		glDisable(GL_TEXTURE_2D);

		glColor4f(0, 0, 0, color[3]);
		glBegin(GL_LINES);
		glVertex3i(x+1, 3, 0);
		glVertex3i(x+1, 15, 0);
		glEnd();

		glEnable(GL_TEXTURE_2D);
	}

	glColor4fv(color);

	return;

/*
	if (G_Text.Text_Changed) {
		ShowCursor(true);
	}

	return;

	while (m_delay > cursor_delay) {
		m_delay -= cursor_delay;
		m_bar = !m_bar;
	}
	if (m_bar && actived) {
		glDisable(GL_TEXTURE_2D);
		glColor3f(0,0,0);
		glBegin(GL_LINES);
		glVertex3i(x + pos_x, pos_y + 3, 0);
		glVertex3i(x + pos_x, pos_y + 15, 0);
		glEnd();
		glColor3f(1,1,1);
		glEnable(GL_TEXTURE_2D);
	}
*/
}

bool GUI::TextInput::HandleMouseRelease(int x, int y, int button) {
	SelectFlag = false;
	return(false);
}

bool GUI::TextInput::HandleMouseDown(int x, int y, int button) {
	if (!m_enabled)
		return(false);

	//std::cout << getName() << "::MouseDown (" << x << ", " << y << ")" << std::endl;

	if (!(button & 1))
		return(false);
	SelectFlag = true;

	ClickPos(x);

	if (m_parent != NULL) {
		if (m_parent->getActiveChild() != NULL) {
			//m_parent->getActiveChild()->onLoseFocus();
			m_parent->setActiveChild(this);
			//onGetFocus();
			GUI::Gui& gui = GUI::Gui::getSingleton();
			gui.setFocus(this);
		}
		else {
			return(false);
		}
	}
	else {
		return(false);
	}

	return(true);
}

void GUI::TextInput::onGetFocus() {
	actived = true;
	//SDL_EnableUNICODE(1);
	//SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
}

void GUI::TextInput::onLoseFocus() {
	actived = false;
	//SDL_EnableKeyRepeat(0, 0);
	//SDL_EnableUNICODE(0);
}

bool GUI::TextInput::HandleKeyUp(SDL_Event *sdlEvent, int mod) {

	SDLKey key = sdlEvent->key.keysym.sym;

	if(key == SDLK_LSHIFT || key == SDLK_RSHIFT) 
		SelectFlag = false;

	if (m_parent == NULL)
		return(false);
	return(m_parent->HandleKeyUp(sdlEvent, mod));
}

bool GUI::TextInput::HandleKeyDown(SDL_Event *sdlEvent, int mod) {
	SDLKey key = sdlEvent->key.keysym.scancode;

	if (m_key == key)
    // Determine if shift is held for selection.  
 //   if( (mod & KMOD_LSHIFT) || (mod & KMOD_RSHIFT) )  
	if (key == SDL_SCANCODE_LSHIFT || key == SDL_SCANCODE_RSHIFT)
		SelectFlag = true;  
 
    // Detect keypresses.  
	if(key == SDL_SCANCODE_BACKSPACE)
		RemoveLeft();
	else if(key == SDL_SCANCODE_DELETE) {
		if (SelectFlag)
			RemoveSelected();
		else
			RemoveRight();
	}
	else if(key == SDL_SCANCODE_LEFT)            
		MoveLeft();
	else if(key == SDL_SCANCODE_RIGHT)
		MoveRight();
	else if(key == SDL_SCANCODE_HOME)
		MoveStart();
	else if(key == SDL_SCANCODE_END)
		MoveEnd();
	// Catch modifier key presses.
	else if(key == SDLK_LSHIFT || key == SDLK_RSHIFT);
	else if(key == SDLK_LCTRL || key == SDLK_RCTRL);
	else if(key == SDLK_LALT || key == SDLK_RALT);
	else if(key == SDLK_LGUI || key == SDLK_RGUI);
	else if(key == SDLK_ESCAPE);
	else if(key == SDLK_PRINTSCREEN || key == SDLK_SCROLLLOCK || key == SDLK_PAUSE);
	else if(key >= SDLK_F1 && key <= SDLK_F15);
	else if(key == SDLK_NUMLOCKCLEAR);
	else if(key == SDLK_TAB);
	else if(key == SDLK_MENU);
	else if(key == SDLK_PAGEDOWN || key == SDLK_PAGEUP);
	else if(key == SDLK_UNKNOWN);
	else if(key == SDLK_RETURN);
	// Detect CTRL+A 
	else if( (mod & KMOD_LCTRL) && (key == 97 || key == 65 ) )
		SelectAll();
	else if( (mod & KMOD_RCTRL) && (key == 97 || key == 65 ) )
		SelectAll();
	// Unicode character 
	// TODO: This was removed on SDL 2 port, make sure this is not needed
	//else if(sdlEvent->key.keysym.unicode != 0)
	//	Insert(sdlEvent->key.keysym.unicode);
	// Num pad
	else if(key >= SDLK_KP_0 && key <= SDLK_KP_9)
		Insert(sdlEvent->key.keysym.sym - SDLK_KP_0 + SDLK_0);
	else if(key == SDLK_KP_PERIOD)
		Insert(SDLK_PERIOD);
	else if(key == SDLK_KP_DIVIDE)
		Insert(SDLK_SLASH);
	else if(key == SDLK_KP_MULTIPLY)
		Insert(SDLK_ASTERISK);
	else if(key == SDLK_KP_MINUS)
		Insert(SDLK_MINUS);
	else if(key == SDLK_KP_PLUS)
		Insert(SDLK_PLUS);
	else if(key == SDLK_KP_ENTER);
	else if(key == SDLK_KP_EQUALS)
		Insert(SDLK_EQUALS);
	// Capitalized character  
	else if( (mod & KMOD_RSHIFT) || (mod & KMOD_LSHIFT) ) {
		if ((key >= SDLK_1 && key <= SDLK_9)
		|| (key == SDLK_COLON || key == SDLK_SEMICOLON))
			Insert(sdlEvent->key.keysym.sym - 16);
		else if (key >= SDLK_COMMA && key <= SDLK_SLASH)
			Insert(sdlEvent->key.keysym.sym + 16);
		else if (key == SDLK_AT)
			Insert(SDLK_BACKQUOTE);
		else
			Insert(toupper(sdlEvent->key.keysym.sym));
	}
	// Normal character  
	else
		Insert(sdlEvent->key.keysym.sym);

	printf("Text input:\t");
	//printf("Unicode: %d\t", sdlEvent->key.keysym.unicode);
	printf("Scancode: %3d\t", sdlEvent->key.keysym.scancode);
	printf("Sym: %3d\t", sdlEvent->key.keysym.sym);
	if (sdlEvent->key.keysym.scancode < SDLK_SPACE || sdlEvent->key.keysym.scancode >= SDLK_DELETE)
		printf("Char: [ ]\t");  
	else
		printf("Char: [%c]\t", sdlEvent->key.keysym.scancode);
	printf("SelectFlag: %s", (SelectFlag)?"T":"F");  
	printf("\n");


	if (m_parent == NULL)
		return(false);
	return(m_parent->HandleKeyDown(sdlEvent, mod));
}

//==================================================================================
// �ӽṹkey.keysym.unicode��ȁEַ�E
//==================================================================================
bool GUI::TextInput::Insert(Uint16 UnicodeCh) {
	m_key = (SDLKey)UnicodeCh;
	len++;

	NODE *tmp;
	if (G_Text.Len <= 0)
		return false;

	if (G_Text.Head == NULL) {
		G_Text.Head = G_Text.Start  = G_Text.Current = NewNode(NULL, NULL, UnicodeCh);
		G_Text.Len--;
		return true;
	}

	if (UnicodeCh <= 31) {
		return false;
	}

	if (G_Text.Current != NULL) {
		if (G_Text.Current->Next != NULL) {
			tmp = G_Text.Current->Next;
			G_Text.Current->Next = NewNode(G_Text.Current, tmp, UnicodeCh);
			if (G_Text.Current->Next == NULL) {
				G_Text.Current->Next = tmp;
				return false;
			}

			tmp->Prev = G_Text.Current->Next;
			if (G_Text.Current->Next != NULL) {
				G_Text.Current = G_Text.Current->Next;
			}
		}
		else {
			G_Text.Current->Next = NewNode(G_Text.Current, NULL, UnicodeCh);
			if (G_Text.Current->Next != NULL) {
				G_Text.Current = G_Text.Current->Next;
			}
			else {
				return false;
			}
		}
	}
	else {
		tmp = NewNode(NULL, G_Text.Start, UnicodeCh);
		if (tmp != NULL) {
			G_Text.Start->Prev = tmp;
			G_Text.Head = G_Text.Start = G_Text.Current = tmp;
		}
		else {
			return false;
		}
	}

	CheckPos();

	G_Text.Len--;
	return true;
}
//==================================================================================
// �µĽڵ�E
//==================================================================================
NODE* GUI::TextInput::NewNode(NODE *Front, NODE *Behind, Uint16 UnicodeCh) {
	NODE *Tmp = (NODE*)malloc(sizeof(NODE));
	if (Tmp != NULL) {
		Tmp->UnicodeCh = UnicodeCh;
		Tmp->selected   = false;
		Tmp->Prev = Front;
		Tmp->Next = Behind;
		Tmp->Font_Screen = NULL;

		G_Text.Text_Changed = true;
	}
	return Tmp;
}
//==================================================================================
// �ӽڵ��ȡ�ַ�����ɴ�
//==================================================================================
void GUI::TextInput::GetStringFromNode(void) {       
	NODE *i;
	int  j=0, w, h;

	const sdle::Font* font = GUI::Gui::getSingleton().getDefaultFont();
	if (font == NULL)
		return;

	if (G_Text.Start == NULL) {
		G_Text.m_text[j] = AU('\0');
		return;
	}
	for (i = G_Text.Start; i != NULL; i = i->Next) {
		G_Text.m_text[j++] = i->UnicodeCh;
		G_Text.m_text[j] = AU('\0');
		font->getSize(G_Text.m_text, &w, &h);
		if (w > this->w) {
			G_Text.m_text[j-1] = AU('\0');
			break;
		}

	}

	G_Text.m_text[j] = AU('\0');
}
//==================================================================================
// ɾ�����һ���ַ�E
//==================================================================================
bool GUI::TextInput::RemoveLeft(void) {
	NODE *tmp;

	if( G_Text.Start == NULL || G_Text.Current == NULL ) {
		return false;
	}

	if (G_Text.Current->Prev != NULL) {
		if (G_Text.Current->Prev->Prev != NULL) {
			tmp = G_Text.Current;
			G_Text.Current = tmp->Prev;
			G_Text.Current->Next = tmp->Next;
			if (tmp->Next != NULL) {
				tmp->Next->Prev = G_Text.Current;
			}

			free(tmp);
		}
		else {
			tmp = G_Text.Current;
			G_Text.Current = tmp->Prev;
			G_Text.Current->Next = tmp->Next;
			//G_Text.Start = G_Text.Current;

			free(tmp);
		}
	}
	else {
		tmp = G_Text.Current;
		G_Text.Current = NULL;
		G_Text.Head = G_Text.Start = tmp->Next;
		if (G_Text.Start != NULL) {
			G_Text.Start->Prev = NULL;
		}
		free(tmp);
	}

	CheckPos();
	++G_Text.Len;
	G_Text.Text_Changed = true;
	return true;
}
//==================================================================================
// ɾ���ұ�һ���ַ�E
//==================================================================================
bool GUI::TextInput::RemoveRight(void) {       
	NODE *tmp;

	if (G_Text.Start == NULL) {
		return false;
	}

	if (G_Text.Current != NULL) {
		if (G_Text.Current->Next != NULL) {
			if (G_Text.Current->Next->Next != NULL) {
				tmp = G_Text.Current->Next;
				G_Text.Current->Next = tmp->Next;
				tmp->Next->Prev = G_Text.Current;

				free(tmp);
			}
			else {
				tmp = G_Text.Current->Next;
				G_Text.Current->Next = NULL;

				free(tmp);
			}
		}
		else {
			return false;
		}
	}
	else {
		if (G_Text.Start->Next != NULL) {
			tmp = G_Text.Start->Next;
			G_Text.Head = G_Text.Start = tmp;
			tmp = G_Text.Start->Prev;
			G_Text.Start->Prev = NULL;

			free(tmp);
		}
		else {
			free(G_Text.Start);
			G_Text.Head = G_Text.Start = NULL;
		}
	}

	CheckPos();
	++G_Text.Len;
	G_Text.Text_Changed = true;
	return true;
}

bool GUI::TextInput::RemoveSelected() {
	return true;
}

//==================================================================================
// ���ƶ�һ���ַ�E
//==================================================================================
bool GUI::TextInput::MoveLeft(void) {
	if (G_Text.Start == NULL || G_Text.Current == NULL) {
		return false;
	}
	G_Text.Text_Changed = false;

	if (G_Text.Current->Prev != NULL) {
		G_Text.Current = G_Text.Current->Prev;
	}
	else {
		G_Text.Current = NULL;
	}

	CheckPos();

	return true;
}
//==================================================================================
// ���ƶ�һ���ַ�E
//==================================================================================
bool GUI::TextInput::MoveRight(void) {
	if (G_Text.Start == NULL) {
		return false;
	}

	G_Text.Text_Changed = false;

	if (G_Text.Current == NULL) {
		G_Text.Current = G_Text.Start;
	}
	else {
		if (G_Text.Current->Next != NULL) {
			G_Text.Current = G_Text.Current->Next;
		}
		else {
			return false;
		}
	}

	CheckPos();

	return true;
}
//==================================================================================
// �ƶ����ַ�����ʼ
//==================================================================================
bool GUI::TextInput::MoveStart(void) {
	G_Text.Start = G_Text.Head;
	G_Text.Current = NULL;
	return true;
}
//==================================================================================
// �ƶ����ַ���β
//==================================================================================
bool GUI::TextInput::MoveEnd(void) {
	while( MoveRight() )
		;
	return true;
}
//==================================================================================
// �ͷ��ַ���
//==================================================================================
void GUI::TextInput::FreeString(void) {
	NODE *Tmp;

	G_Text.Start = G_Text.Head;
	while (G_Text.Start != NULL) {
		Tmp = G_Text.Start->Next;
		free(G_Text.Start);
		G_Text.Start = Tmp;
	}
	free(G_Text.m_text);
}
//==================================================================================
// ��ȡ���X����E
//==================================================================================
int GUI::TextInput::GetCursorX() {
	NODE *i;
	int k = 0, w, h;
	Uint16 *Buffer = NULL;

	Buffer = new Uint16[MaxLen+1];

	const sdle::Font* font = GUI::Gui::getSingleton().getDefaultFont();
	if (font == NULL)
		return(0);


	if (Buffer == NULL)
		return 0;

	if (G_Text.Start != NULL && G_Text.Current != NULL) {
		for (i = G_Text.Start; i != G_Text.Current->Next; i = i->Next) {
			Buffer[k++] = i->UnicodeCh;
			Buffer[k] = AU('\0');
			font->getSize(Buffer, &w, &h);
			if (w > this->w) {
				k--;
				break;
			}
		}
	}
	Buffer[k] = AU('\0');
	font->getSize(Buffer, &w, &h);
	delete []Buffer;
	return (w);
}
//==================================================================================
// �ж����Ҽ���
//==================================================================================
bool GUI::TextInput::CheckPos() {
	int Step = 3;
	NODE *i;
	int k = 0, w, h;
	Uint16 *Buffer = NULL;

	const sdle::Font* font = GUI::Gui::getSingleton().getDefaultFont();
	if (font == NULL)
		return(false);

	if (G_Text.Start == NULL || G_Text.Current == NULL) {
		return false;
	}

	if (G_Text.Current == G_Text.Start) {
		for(int i = 0; i < Step; i++) {
			if (G_Text.Start->Prev == NULL) {
				break;
			}
			else {
				G_Text.Start = G_Text.Start->Prev;
			}
		}
		return true;
	}

	Buffer = new Uint16[MaxLen+1];

	if (Buffer == NULL)
		return false;

	for (i = G_Text.Start; i != NULL; i = i->Next) {
		Buffer[k++] = i->UnicodeCh;
		if(i == G_Text.Current->Next)
			break;
	}

	Buffer[k] = AU('\0');
	font->getSize(Buffer, &w, &h);
	delete []Buffer;
	if (w > this->w) {
		if (G_Text.Current->Next == NULL) {
			Step = 2;
		}

		for(int i = 0; i < Step ;i++) {
			if (G_Text.Start->Next != NULL)
				G_Text.Start = G_Text.Start->Next;
			else
				break;
		}
		return true;
	}

	return false;
}
//==================================================================================
// ��ȡ��E㻁E
//==================================================================================
void GUI::TextInput::ClickPos(int x) {
	NODE *i;
	int k = 0, w, h;
	Uint16 *Buffer = NULL;

	const sdle::Font* font = GUI::Gui::getSingleton().getDefaultFont();
	if (font == NULL)
		return;

	if (G_Text.Start == NULL) {
		return;
	}

	Buffer = new Uint16[MaxLen+1];

	if (Buffer == NULL)
		return;

	for (i = G_Text.Start; i != NULL; i=i->Next) {
		Buffer[k++] = i->UnicodeCh;
		Buffer[k] = AU('\0');
		font->getSize(Buffer, &w, &h);
		if (w > this->w) {
			G_Text.Current = i->Prev;
			break;
		}
		else if (w > x) {
			G_Text.Current = i->Prev;
			break;
		}
		else {
			G_Text.Current = i;
		}
	}

	delete []Buffer;
	CheckPos();
}

std::string& GUI::TextInput::getText() {
	NODE *i;
	int  j=0;
	LCHAR *Buffer = NULL;

	m_text.clear();

	Buffer = new LCHAR[MaxLen+1];

	if (Buffer == NULL) {
		return m_text;
	}
	if (G_Text.Start == NULL) {
		return m_text;
	}
	char *mbCh = new char[MB_CUR_MAX+1];
	for (i = G_Text.Start; i != NULL; i = i->Next) {
		wchar_t wideCh = wchar_t(i->UnicodeCh);
		int mbLen = wctomb(mbCh, wideCh);
		if (mbLen == -1) {// invalid multibyte character
			m_text.clear();
			return m_text;
		}
		mbCh[mbLen] = '\0';
		m_text.append(mbCh);
		G_Text.m_text[j++] = i->UnicodeCh;
	}
	delete []mbCh;

	return m_text;
}

void GUI::TextInput::SelectAll() {
	selStart = 0;
	selEnd = len;

	NODE *i;

	MoveEnd();
	G_Text.select1 = G_Text.Head;
	G_Text.select2 = G_Text.Current;
	i = G_Text.Head;
	while (i != NULL)
	{
		i->selected = true;
		i = i->Next;
	}
}

void GUI::TextInput::UnSelectAll() {
	selStart = selEnd = 0;

	NODE *i;

	i = G_Text.Head;
	while (i != NULL) {
		i->selected = false;
		i = i->Next;
	}
}
