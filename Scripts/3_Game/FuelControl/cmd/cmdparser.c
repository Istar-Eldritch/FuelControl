class CmdArgs {
    ref array<string> positional;
    ref map<string, ref array<string>> mapped;
    ref map<string, int> flags;

    void CmdArgs() {
        positional = new ref array<string>;
        mapped = new ref map<string, ref array<string>>;
        flags = new ref map<string, int>;
    }

    static ref CmdArgs FromTokens(ref array<string> tokens) {
        ref CmdArgs args = new ref CmdArgs();
        string key;
        int i;
        for (i = 0; i < tokens.Count(); i++) {
            auto current = tokens.Get(i);
            if (current[0] == "-" && tokens.Get(i + 1) == "=") {
                key = current[0].Substring(1, current[0].Length());
                string value = tokens.Get(i + 2);

                auto existing_values = args.mapped.Get(key);
                if (!existing_values) {
                    existing_values = new ref array<string>;
                }
                existing_values.Insert(value);
                args.mapped.Insert(key, existing_values);
                i = i + 2;
            } else if (current[0] == "-") {
                key = current[0].Substring(1, current[0].Length());
                int existing_count = args.flags.Get(key);
                args.flags.Insert(key, existing_count + 1);
            } else {
                args.positional.Insert(current);
            }
        }

        return args;
    }
}

class CmdParser {

    static ref Param2<string, string> TokenizeQuotedString(string cmd) {
        if (cmd.Length() > 0 && cmd[0] == "\"") {
            int i;
            string acc = "";
            for (i = 1; i < cmd.Length(); i++) {
                if (cmd[i] == "\"") {
                    return new ref Param2<string, string>(acc, cmd.Substring(i + 1, cmd.Length()));
                }
                acc = acc + cmd[i];
            }
        }

        return null;
    }

    static ref Param2<string, string> TokenizeEq(string cmd) {
        if (cmd.Length() > 0 && cmd[0] == "=") {
            return new ref Param2<string, string>("=", cmd.Substring(1, cmd.Length()));
        }

        return null;
    }

    static ref Param2<string, string> TokenizeString(string cmd) {
        if (cmd.Length() > 0 && cmd[0] != " ") {
            int i;
            string acc = "";
            for (i = 0; i < cmd.Length(); i++) {
                if (cmd[i] == " " || cmd[i] == "=") {
                    break;
                }
                acc = acc + cmd[i];
            }
            return new ref Param2<string, string>(acc, cmd.Substring(acc.Length(), cmd.Length()));
        }

        return null;
    }

    static ref Param2<string, string> TokenizeSpace(string cmd) {
        if (cmd.Length() > 0) {
            int i;
            string acc = "";
            for (i = 0; i < cmd.Length(); i++) {
                if (cmd[i] != " ") {
                    return new ref Param2<string, string>(acc, cmd.Substring(i, cmd.Length()));
                }
                acc = acc + cmd[i];
            }
        }

        return null;
    }

    static ref array<string> Tokenize(string cmd) {
        ref array<string> tokens = new ref array<string>;
        string processing = cmd;
        while (processing.Length() > 0) {
            ref Param2<string, string> parseResult;
            parseResult = TokenizeQuotedString(processing);
            if(parseResult) {
                tokens.Insert(parseResult.param1);
                processing = parseResult.param2;
            }

            parseResult = TokenizeString(processing);
            if(parseResult) {
                tokens.Insert(parseResult.param1);
                processing = parseResult.param2;
            }

            parseResult = TokenizeEq(processing);
            if(parseResult) {
                tokens.Insert(parseResult.param1);
                processing = parseResult.param2;
            }


            parseResult = TokenizeSpace(processing);
            if(parseResult) {
                processing = parseResult.param2;
            }

        }

        return tokens;
    }

    static ref CmdArgs Parse(string cmd) {
        ref array<string> tokens = Tokenize(cmd);
        if(tokens.Count() > 0) {
            return CmdArgs.FromTokens(tokens);
        }
        return null;
    }

}