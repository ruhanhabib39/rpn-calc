#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
struct RPNCalculator {
private:
	stack<ll> nums; // stack of numbers
public:
	// Is there a better way?
	// Fn represents the operator's function
	class Fn {
	public:
		virtual void operator()(stack<ll>&) = 0;
		virtual ~Fn() {
		}
	};
	// Binary function: some part, such as stack management simplified
	// TODO: Merge BinFn and BinOp together.
	class BinFn : public Fn {
	public:
		virtual void operator()(stack<ll>& st) {
			// a b fn
			// the top number on stack is b, the next one is a. pop them and replace them with fn(a, b)
			if(st.size() != 2) throw ArgCountMismatch(2, st.size());
			ll b = st.top(); st.pop();
			ll a = st.top(); st.pop();
			st.push(fn(a, b));
		}
		virtual ll fn(ll, ll) = 0; // virtual function to be implemented by child classes
	};
	// BinOp functions just wraps every function class, so that's its easier to use current functions.
	template<class Op>
	class BinOp : public BinFn {
	public:
		virtual ll fn(ll a, ll b) {
			return Op()(a, b);
		}
	};
	// power function class for any type T
	template<class T> // T is a number
	class power {
	public:
		// O(n) code.. because for ll, for example pow(2, 64) > range of ll
		T operator()(T b, T p) {
			T ml = 1;
			for(T i = 1; i <= p; i++) ml *= b;
			return ml;
		}
	};
	struct Exception : std::exception {
	};
	struct InvalidRPN : Exception {
		virtual const char* what() const noexcept override {
			return "Invalid RPN!";
		}
	};
	struct OperatorNotFound : Exception {
		string msg;
		OperatorNotFound(char op) {
			msg = string("Operator ") + op + string(" not found");
		}
		virtual const char* what() const noexcept override {
			return msg.c_str();
		}
	};
	struct ArgCountMismatch : Exception {
		int required, got;
		ArgCountMismatch(int r, int g) {
			required = r;
			got = g;
		}
		virtual const char* what() const noexcept override {
			string msg = string("ArgCountMismatch: required ") + to_string(required)
						+ string(", got: ") + to_string(got);
			return msg.c_str();
		}
	};
	unordered_map<char,Fn*> funcs;
	RPNCalculator() {
		funcs['+'] = new BinOp<plus<ll>>();
		funcs['-'] = new BinOp<minus<ll>>();
		funcs['*'] = new BinOp<multiplies<ll>>();
		funcs['/'] = new BinOp<divides<ll>>();
		funcs['%'] = new BinOp<modulus<ll>>();
		funcs['^'] = new BinOp<power<ll>>();
	}
	~RPNCalculator() {
		for(auto it : funcs) delete it.second;
	}
	void push(ll num) {
		nums.push(num);
	}
	void push(char op) {
		auto it = funcs.find(op);
		if(it == funcs.end()) throw OperatorNotFound(op);
		(*funcs[op])(nums);
	}
	ll getVal() {
		if(nums.size() != 1) throw InvalidRPN();
		return nums.top();
	}
private:
	static ll toNum(char c) {
		return c - '0';
	}
	static ll parseNum(string::const_iterator& it, string::const_iterator e) {
		ll num = 0;
		const ll base = 10;
		while(it != e and isdigit(*it)) {
			num *= base; // assuming num is in base 10
			num += toNum(*it);
			++it;
		}
		return num;
	}
public:
	static ll calc(const string& s) {
		RPNCalculator calc;
		try {
			for(auto it = s.begin(); it != s.end();) {
				if(*it == '#') break;
				if(isdigit(*it)) {
					calc.push(parseNum(it, s.end()));
				} else if(isspace(*it)) {
					++it;
				} else {
					calc.push(*it);
					++it;
				}
			}
			return calc.getVal();
		} catch(const RPNCalculator::Exception& rp) {
			cerr << "Error: " << rp.what() << "\n";
			exit(-1);
		}
	}
};

void displayMyLogo() {
	cout << "||===||\n";
	cout << "||   ||\n";
	cout << "||===\\\\\n";
	cout << "||    \\\\\n";
	cout << "By Ruhan Habib(email: ruhanhabib39@gmail.com)\n";
}

void sayBye() {
	cout << "Bye Bye!\n";
	exit(0);
}

void cmdClear() {
	#ifdef _WIN32
		system("cls");
	#else
		system("clear");
	#endif
}

int main(int argc, char* argv[]) {
	ios_base::sync_with_stdio(false);
	string execName = argv[0];
	if(argc == 2 and strcmp(argv[1], "--help") == 0) {
		displayMyLogo();
		cout << "Usage: " << execName << " [options]\n";
		cout << "This is a reverse polish notation calculator. For example, 2 + 3 is an invalid here\n";
		cout << "eg. 2 3 + # returns 2 + 3, which is 5\n";
		cout << "Options:\n";
		cout << "\t--help\tDisplays this help message\n";
		cout << "\t-e\t Execute the RPN-Notation, passed as argument. eg: " << execName << " -e \"2 3 +\"\n";
		sayBye();
	} else if(argc == 3 and strcmp(argv[1], "-e") == 0) {
		cout << RPNCalculator::calc(string(argv[2])) << endl;
		exit(0);
	} else if(argc >= 2) {
		cerr << "Only --help and -e are valid arguments for now\n";
		exit(-1);
	}
	displayMyLogo();
	cout << "eg. 2 3 + # returns 2 + 3, which is 5\n";
	string inp;
	while(true) {
		cout << "> ";
		getline(cin, inp);
		for(char& c : inp) c = tolower(c);
		if(inp == "bye" or inp == "quit" or inp == "exit") sayBye();
		else if(inp == "clear") cmdClear();
		else cout << RPNCalculator::calc(inp) << endl;
	}
	return 0;
}
