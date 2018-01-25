defmodule TimeServer do
  require Logger

  def main(args \\ []) do
    args
    |> parse_args
    |> execute

    # keep the process running indefinitely
    Stream.timer(:infinity) |> Enum.to_list
  end

  def parse_args(args) do
    {opts, extra, errors} = 
      args
      |> OptionParser.parse(strict: [port: :integer],
                            aliases: [p: :port, port: :port])
      # strict will raise errors if the args are not present.
      # aliases will overwrite the default arg format, which is --arg, to -alias.
      # so --port is mapped to -port and -p.
    case errors do
      [] -> {opts}
      _ -> print_usage
    end
  end
  
  def print_usage() do
    IO.puts "Usage: TimeServer [-port <port#>]"
  end

  def execute({opts}) do
    if opts[:port] == nil do
      Logger.info "Starting server on port 5005"
      TimeServer.Supervisor.start_link(5005)
    else
      Logger.info "Starting server on port #{opts[:port]}"
      TimeServer.Supervisor.start_link(opts[:port])
    end
  end
end
