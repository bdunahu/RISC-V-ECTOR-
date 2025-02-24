from code import InteractiveConsole

def start(user):
    header =f"Press C-d to close."
    footer = f"See you later, {user}!"

    scope_vars = {"cache_size": 12}

    InteractiveConsole(locals=scope_vars).interact(header, "")
    return footer
