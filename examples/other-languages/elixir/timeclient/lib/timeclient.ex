defmodule TimeClient do
  require Logger

  def main(args \\ []) do
    args
    |> parse_args
    |> execute
  end

  def parse_args(args) do
    if args == [], do: print_usage
    {opts, extra, errors} = args |>
        OptionParser.parse(switches: [verbose: :boolean], aliases: [verbose: :verbose, v: :verbose])
    unless opts[:verbose], do: Logger.configure([level: :error])
    case errors do
      [] -> 
        case extra do
          [serverhost, port] -> extra
          _ -> print_usage
        end
      _ -> print_usage
    end
  end

  def execute([serverhost, port]) do
    TimeClient.Client.start_link(serverhost, port)
  end

  defp print_usage() do
    IO.puts "Usage: TimeClient [-verbose] <serverhost> <port>"
    Kernel.exit(:shutdown)
  end
end
